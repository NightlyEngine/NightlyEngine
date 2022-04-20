# Set console colors
class colors:
    OKCYAN = '\033[96m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'


def log_info(msg):
    print(colors.BOLD + colors.OKCYAN + "==> " + msg + colors.ENDC)

def log_fail(msg):
    print(colors.FAIL + msg + colors.ENDC)

