class BookingError(Exception):
    def __init__(self, name, msg):
        self.name = name
        self.msg = msg