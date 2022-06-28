import sysv_ipc
import numpy as np
import struct

BUFF_SIZE_RECIEVER = 16
BUFF_SIZE_SENDER = 32
while True:
    try:
        mq_reciever = sysv_ipc.MessageQueue(1, sysv_ipc.IPC_CREAT)

        message, mtype = mq_reciever.receive()
        if mtype == 1:
            afl_fsrv_map_size = np.frombuffer(message, dtype=np.float64)
            # afl_fsrv_map_size = message.decode()
            print(f"afl->fsrv.map_size: {afl_fsrv_map_size}")

    except sysv_ipc.ExistentialError:
        print("ERROR: message queue creation failed")

    msg_npy = np.arange(BUFF_SIZE_SENDER, dtype=np.uint8).reshape((2,BUFF_SIZE_SENDER//2))

    try:
        mq_sender = sysv_ipc.MessageQueue(2, sysv_ipc.IPC_CREAT)
        TYPE = 1
        mq_sender.send(msg_npy.tobytes(order='C'), True, type=TYPE)

    except sysv_ipc.ExistentialError:
        print("ERROR: message queue creation failed")