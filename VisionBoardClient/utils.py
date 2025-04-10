from PySide6.QtCore import QByteArray, QDataStream, QIODevice

def create_packet(packet_type, *args):
    """
    Create a network-safe packet.
    Handles normal text packets and raw binary image data.
    """
    if packet_type == "IMG" and isinstance(args[0], bytes):
        # Assume args = (encoded_image_bytes,)
        return f"{packet_type}|".encode() + args[0]

    # Make sure all parts are strings for joining
    str_args = [str(arg) for arg in args]
    packet = f"{packet_type}|{'|'.join(str_args)}"
    return packet.encode()


def create_raw_packet(header, payload):
    return f"{header}|{payload}".encode()
