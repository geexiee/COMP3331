import pytest as p
from Location import *
from CarRentalSystem import *
from BookingError import *
from datetime import datetime


def setup_module():
    print("Setting up variables...")
    p.sys = CarRentalSystem()
    p.customer = "Matt"
    p.car = "Mazda"
    date_format = "%Y-%m-%d"
    start = "1999-04-02"
    end = "1999-04-03"
    start_date = datetime.strptime(start, date_format)
    end_date = datetime.strptime(end, date_format)
    p.period = end_date - start_date
    pickup = "hello"
    dropoff = "world"
    p.location = Location(pickup, dropoff)


def teardown_module():
    print("Tests completed.")


def test_pickup_entered():
    print("Testing for pickup...")
    pickup = ""
    dropoff = "world"
    location = Location(pickup, dropoff)
    try:
        p.sys.make_booking(p.customer, p.period, p.car, location)
    except BookingError as msg:
        assert msg.name == "location"

def test_dropoff_entered():
    print("Testing for dropoff...")
    pickup = ""
    dropoff = "world"
    location = Location(pickup, dropoff)
    try:
        p.sys.make_booking(p.customer, p.period, p.car, location)
    except BookingError as msg:
        assert msg.name == "location"

def test_period_entered():
    start = "1999-04-02"
    end = "1999-04-01"
    date_format = "%Y-%m-%d"
    start_date = datetime.strptime(start, date_format)
    end_date = datetime.strptime(end, date_format)
    period = end_date - start_date

    try:
        p.sys.make_booking(None, period, None, None)
    except BookingError as msg:
        assert msg.name == "period"

