class BookingError(Exception):
	# Initialiser
	def __init__(self, carname):
		self.carname = carname
	try:
		carname = input("What car are you looking for nibba?\n")
        if (carname 
	except ValueError:
		print("This aint a legit car yo\n")

