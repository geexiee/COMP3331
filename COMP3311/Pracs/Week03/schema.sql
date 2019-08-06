-- COMP3311 Prac 03 Exercise
-- Schema for simple company database
create table Employees (
	tfn         char(11),
	givenName   varchar(30) NOT NULL,
	familyName  varchar(30),
	hoursPweek  float,
	CHECK (hoursPweek <=168),
	CHECK (hoursPweek > 0),
	PRIMARY KEY (tfn),
	CHECK (tfn LIKE '[0-9] [0-9] [0-9]-[0-9] [0-9] [0-9]-[0-9] [0-9] [0-9]')
);

create table Departments (
	id          char(3),
	name        varchar(100) UNIQUE,
	manager     char(11) UNIQUE,
	PRIMARY KEY (id),
	CHECK (id LIKE '[0-9][0-9][0-9]')
);

create table DeptMissions (
	department  char(3),
	keyword     varchar(20) NOT NULL,
	FOREIGN KEY (department) REFERENCES Departments(id)
);

create table WorksFor (
	employee    char(11) NOT NULL,
	department  char(3) NOT NULL,
	percentage  float,
	CHECK (percentage > 0),
	CHECK (percentage <= 100),
	FOREIGN KEY (department) REFERENCES Departments(id)
);
