create table book
  (bno char(10), 
  category 	varchar(10),
  title 	varchar(20),
  press	varchar(20),
  year int,
  author varchar(10),
  price	decimal(7,2),
  total	int,
  stock	int,
  primary key(bno));

create table card
  (cno char(7),
  name varchar(10),
  department varchar(40),
  type char(1),
  primary key(cno),
  check(type in('T','S')));

create table borrow
  (cno char(7),
  bno  char(8),
  borrow_date int,
  return_date int,
  primary key(cno,bno),
  foreign key (cno) references card(cno),
  foreign key (bno) references book(bno));

create table student
  (
  name char(10),
  sex char(1),
  age int
  );

create table account
  (
    username varchar(20),
      password varchar(20),
      primary key(username)
  );

insert into account values('root', '123456');
insert into account values('null','null');