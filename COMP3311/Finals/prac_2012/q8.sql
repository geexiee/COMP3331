create table employee (
    id integer primary key,
    name text,
    position text,
);

create table part-time (
    fraction float,
    id integer,
    foreign key (id) references employee(id)
);

create table casual (
    id integer,
    foreign key (id) references employee(id)
);

create table hours (
    starting time,
    ending time,
    date date,
    primary key (starting, ending, date)
)

create table worked (
    id integer,
    starting time,
    ending time,
    date date,
    primary key(id, starting, ending, date)
    foreign key (id) references casual(id)
);
    
