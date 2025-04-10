def create_packet(packet_type, payload):
    return f"{packet_type}|{payload}".encode()

def parse_packet(data):
    try:
        raw = data.decode()
        packet_type, payload = raw.split("|", 1)
        return packet_type, payload
    except:
        return "INVALID", ""
