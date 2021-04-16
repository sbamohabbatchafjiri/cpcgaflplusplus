#include "frida-gum.h"

#include "api.h"
#include "config.h"

#include "instrument.h"
#include "persistent.h"

#if defined(__x86_64__)

typedef struct x86_64_regs arch_api_regs;

static arch_api_regs saved_regs = {0};
static void *        saved_return = NULL;

gboolean persistent_is_supported() {

  return true;

}

static void instrument_persitent_save_regs(GumX86Writer *      cw,
                                           struct x86_64_regs *regs) {

  GumAddress regs_address = GUM_ADDRESS(regs);
  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP,
                                        -(GUM_RED_ZONE_SIZE));

  /* Should be pushing FPU here, but meh */
  gum_x86_writer_put_pushfx(cw);
  gum_x86_writer_put_push_reg(cw, GUM_REG_RAX);

  gum_x86_writer_put_mov_reg_address(cw, GUM_REG_RAX, regs_address);

  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 1),
                                            GUM_REG_RBX);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 2),
                                            GUM_REG_RCX);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 3),
                                            GUM_REG_RDX);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 4),
                                            GUM_REG_RDI);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 5),
                                            GUM_REG_RSI);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 6),
                                            GUM_REG_RBP);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 7),
                                            GUM_REG_R8);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 8),
                                            GUM_REG_R9);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 9),
                                            GUM_REG_R10);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 10),
                                            GUM_REG_R11);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 11),
                                            GUM_REG_R12);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 12),
                                            GUM_REG_R13);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 13),
                                            GUM_REG_R14);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 14),
                                            GUM_REG_R15);

  /* Store RIP */
  gum_x86_writer_put_mov_reg_address(cw, GUM_REG_RBX,
                                     GUM_ADDRESS(persistent_start));

  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 15),
                                            GUM_REG_RBX);

  /* Store adjusted RSP */
  gum_x86_writer_put_mov_reg_reg(cw, GUM_REG_RBX, GUM_REG_RSP);

  /* RED_ZONE + Saved flags, RAX, alignment */
  gum_x86_writer_put_add_reg_imm(cw, GUM_REG_RBX,
                                 GUM_RED_ZONE_SIZE + (0x8 * 3));
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 16),
                                            GUM_REG_RBX);

  /* Save the flags */
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RBX, GUM_REG_RSP, 0x8);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 17),
                                            GUM_REG_RBX);

  /* Save the RAX */
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RBX, GUM_REG_RSP, 0x0);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, (0x8 * 0),
                                            GUM_REG_RBX);

  /* Pop the saved values */
  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP, 0x10);

  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP,
                                        (GUM_RED_ZONE_SIZE));

}

static void instrument_persitent_restore_regs(GumX86Writer *      cw,
                                              struct x86_64_regs *regs) {

  GumAddress regs_address = GUM_ADDRESS(regs);
  gum_x86_writer_put_mov_reg_address(cw, GUM_REG_RAX, regs_address);

  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RCX, GUM_REG_RAX,
                                            (0x8 * 2));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RDX, GUM_REG_RAX,
                                            (0x8 * 3));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RDI, GUM_REG_RAX,
                                            (0x8 * 4));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RSI, GUM_REG_RAX,
                                            (0x8 * 5));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RBP, GUM_REG_RAX,
                                            (0x8 * 6));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_R8, GUM_REG_RAX,
                                            (0x8 * 7));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_R9, GUM_REG_RAX,
                                            (0x8 * 8));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_R10, GUM_REG_RAX,
                                            (0x8 * 9));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_R11, GUM_REG_RAX,
                                            (0x8 * 10));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_R12, GUM_REG_RAX,
                                            (0x8 * 11));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_R13, GUM_REG_RAX,
                                            (0x8 * 12));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_R14, GUM_REG_RAX,
                                            (0x8 * 13));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_R15, GUM_REG_RAX,
                                            (0x8 * 14));

  /* Don't restore RIP or RSP */

  /* Restore RBX, RAX & Flags */
  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP,
                                        -(GUM_RED_ZONE_SIZE));

  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RBX, GUM_REG_RAX,
                                            (0x8 * 1));
  gum_x86_writer_put_push_reg(cw, GUM_REG_RBX);

  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RBX, GUM_REG_RAX,
                                            (0x8 * 0));
  gum_x86_writer_put_push_reg(cw, GUM_REG_RBX);
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RBX, GUM_REG_RAX,
                                            (0x8 * 17));
  gum_x86_writer_put_push_reg(cw, GUM_REG_RBX);

  gum_x86_writer_put_popfx(cw);
  gum_x86_writer_put_pop_reg(cw, GUM_REG_RAX);
  gum_x86_writer_put_pop_reg(cw, GUM_REG_RBX);

  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP,
                                        (GUM_RED_ZONE_SIZE));

}

static void instrument_save_ret(GumX86Writer *cw, void **saved_return_ptr) {

  GumAddress saved_return_address = GUM_ADDRESS(saved_return_ptr);
  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP,
                                        -(GUM_RED_ZONE_SIZE));
  gum_x86_writer_put_push_reg(cw, GUM_REG_RAX);
  gum_x86_writer_put_push_reg(cw, GUM_REG_RBX);

  gum_x86_writer_put_mov_reg_address(cw, GUM_REG_RAX, saved_return_address);
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RBX, GUM_REG_RSP,
                                            GUM_RED_ZONE_SIZE + 0x10);
  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RAX, 0, GUM_REG_RBX);

  gum_x86_writer_put_pop_reg(cw, GUM_REG_RBX);
  gum_x86_writer_put_pop_reg(cw, GUM_REG_RAX);

  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP,
                                        (GUM_RED_ZONE_SIZE));

}

static void instrument_jump_ret(GumX86Writer *cw, void **saved_return_ptr) {

  GumAddress saved_return_address = GUM_ADDRESS(saved_return_ptr);
  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP,
                                        -(GUM_RED_ZONE_SIZE));

  /* Place holder for ret */
  gum_x86_writer_put_push_reg(cw, GUM_REG_RAX);
  gum_x86_writer_put_push_reg(cw, GUM_REG_RAX);

  gum_x86_writer_put_mov_reg_address(cw, GUM_REG_RAX, saved_return_address);
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RAX, GUM_REG_RAX, 0);

  gum_x86_writer_put_mov_reg_offset_ptr_reg(cw, GUM_REG_RSP, 0x8, GUM_REG_RAX);
  gum_x86_writer_put_pop_reg(cw, GUM_REG_RAX);
  gum_x86_writer_put_ret_imm(cw, GUM_RED_ZONE_SIZE);

}

static int instrument_afl_persistent_loop_func() {

  int ret = __afl_persistent_loop(persistent_count);
  previous_pc = 0;
  return ret;

}

static int instrument_afl_persistent_loop(GumX86Writer *cw) {

  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP,
                                        -(GUM_RED_ZONE_SIZE));
  gum_x86_writer_put_call_address_with_arguments(
      cw, GUM_CALL_CAPI, GUM_ADDRESS(instrument_afl_persistent_loop_func), 0);
  gum_x86_writer_put_test_reg_reg(cw, GUM_REG_RAX, GUM_REG_RAX);

  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP,
                                        (GUM_RED_ZONE_SIZE));

}

static void persistent_prologue_hook(GumX86Writer *      cw,
                                     struct x86_64_regs *regs) {

  if (hook == NULL) return;
  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP,
                                        -(GUM_RED_ZONE_SIZE));

  gum_x86_writer_put_mov_reg_address(cw, GUM_REG_RCX,
                                     GUM_ADDRESS(__afl_fuzz_len));
  gum_x86_writer_put_mov_reg_reg_offset_ptr(cw, GUM_REG_RCX, GUM_REG_RCX, 0);
  gum_x86_writer_put_mov_reg_u64(cw, GUM_REG_RDI, 0xffffffff);
  gum_x86_writer_put_and_reg_reg(cw, GUM_REG_RCX, GUM_REG_RDI);

  gum_x86_writer_put_call_address_with_arguments(
      cw, GUM_CALL_CAPI, GUM_ADDRESS(hook), 4, GUM_ARG_ADDRESS,
      GUM_ADDRESS(regs), GUM_ARG_ADDRESS, GUM_ADDRESS(0), GUM_ARG_ADDRESS,
      GUM_ADDRESS(__afl_fuzz_ptr), GUM_ARG_REGISTER, GUM_REG_RCX);

  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP,
                                        (GUM_RED_ZONE_SIZE));

}

void persistent_prologue(GumStalkerOutput *output) {

  /*
   *  SAVE REGS
   *  SAVE RET
   *  POP RET
   * loop:
   *  CALL instrument_afl_persistent_loop
   *  TEST EAX, EAX
   *  JZ end:
   *  call hook (optionally)
   *  RESTORE REGS
   *  call original
   *  jmp loop:
   *
   * end:
   *  JMP SAVED RET
   *
   * original:
   *  INSTRUMENTED PERSISTENT FUNC
   */

  GumX86Writer *cw = output->writer.x86;

  gconstpointer loop = cw->code + 1;
  // gum_x86_writer_put_breakpoint(cw);

  /* Stack must be 16-byte aligned per ABI */
  instrument_persitent_save_regs(cw, &saved_regs);

  /* Stash and pop the return value */
  instrument_save_ret(cw, &saved_return);
  gum_x86_writer_put_lea_reg_reg_offset(cw, GUM_REG_RSP, GUM_REG_RSP, (8));

  /* loop: */
  gum_x86_writer_put_label(cw, loop);

  /* call instrument_prologue_func */
  instrument_afl_persistent_loop(cw);

  /* jz done */
  gconstpointer done = cw->code + 1;
  gum_x86_writer_put_jcc_near_label(cw, X86_INS_JE, done, GUM_UNLIKELY);

  /* Optionally call the persistent hook */
  persistent_prologue_hook(cw, &saved_regs);

  instrument_persitent_restore_regs(cw, &saved_regs);
  gconstpointer original = cw->code + 1;
  /* call original */
  gum_x86_writer_put_call_near_label(cw, original);
  /* jmp loop */
  gum_x86_writer_put_jmp_near_label(cw, loop);

  /* done: */
  gum_x86_writer_put_label(cw, done);

  instrument_jump_ret(cw, &saved_return);

  /* original: */
  gum_x86_writer_put_label(cw, original);

  gum_x86_writer_flush(cw);

}

#endif

