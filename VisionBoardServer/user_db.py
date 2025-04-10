
def verify_user(username, password):
    try:
        with open("users.txt", "r") as f:
            for line in f:
                u, p = line.strip().split("|")
                if u == username and p == password:
                    return True
    except FileNotFoundError:
        return False
    return False
