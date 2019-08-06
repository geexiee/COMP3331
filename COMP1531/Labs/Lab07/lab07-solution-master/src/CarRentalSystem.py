from src.Booking import Booking


class CarRentalSystem:
    def __init__(self):
        self._cars = []
        self._customers = []
        self._bookings = []

    def car_search(self, name=None, model=None):
        #  pass
        if name is None and model is None:
            return self._cars
        cars = []

        for car in self._cars:
            c_name = car.get_name()
            c_model = car.get_model()
            if name is not None and name.lower() in c_name.lower():
                cars.append(car)
            elif model is not None and model.lower() in c_model.lower():
                cars.append(car)
        return cars

    def make_booking(self, customer, period, car, location):
    
        if location.pickup = NULL || location.dropoff = NULL:
            return
        if period.end is before period.start:
            return
        try:
            if location is NULL:
                raise BookingError(customer, period, car, NULL)
            if period is NULL: 
                raise BookingError(customer, NULL, car, location)
                
        new_booking = Booking(customer, period, car, location)
        self._bookings.append(new_booking)
        car.add_booking(new_booking)
        return new_booking

    class BookingError(Exception):
        def __init__(self, customer, period = NULL, car, location = NULL):
            if period is NULL:
                print("Specify a valid period\n")
            if location is NULL:
                print("Specify a valid location\n")               
    
    def get_customer(self, username):
        """
        Just returns the first customer, should do a search but not
        needed for this use case. Will break if no customers in list
        """
        return self._customers[0]

    def add_car(self, car):
        self._cars.append(car)

    def new_customer(self, customer):
        self._customers.append(customer)

    def validate_login(self, username, password):
        for c in self._customers:
            if c.username == username and c.validate_password(password):
                return c
        return None

    def get_user_by_id(self, user_id):
        for c in self._customers:
            if c.get_id() == user_id:
                return c
        return None

    @property
    def cars(self):
        return self._cars

    def get_car(self, rego):
        for c in self._cars:
            if c.get_rego() == rego:
                return c
        return None
