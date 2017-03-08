USE ClubcDb;

CREATE TABLE tblClubcUserClass
(
	Class int NOT NULL PRIMARY KEY,
	Descript nvarchar(50) NOT NULL UNIQUE
);

CREATE TABLE tblClubcUsers
(
	ID nvarchar(50) NOT NULL PRIMARY KEY,
	Pass nvarchar(50) NOT NULL,
	Class nvarchar(50) NOT NULL REFERENCES tblClubcUserClass(Descript),
	Nickname nvarchar(50) NOT NULL UNIQUE
);

delimiter //
CREATE PROCEDURE CreateClubcUser (
	User NVARCHAR(50),
	Passwd NVARCHAR(50),
	Class NVARCHAR(50),
	Nickname NVARCHAR(50)
)
BEGIN
	INSERT INTO tblClubcUsers VALUES ( User, Passwd, Class, Nickname );
END//
delimiter ;

delimiter //
CREATE PROCEDURE ClubcSvrLogin (
	UserName NVARCHAR(50),
	Passwd NVARCHAR(50)
)
BEGIN
	select * from tblClubcUsers where ID = UserName AND Pass = Passwd;
END//
delimiter ;

