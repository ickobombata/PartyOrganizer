# MySQL environment setup

* Install packages
```sudo apt-get install mysql-client libmysqlclient-dev mysql-server```

* Once installed:
```sudo mysql```

Inside of the mysql console:

* Create user with privileges
```
mysql> CREATE USER 'kurendo'@'localhost' IDENTIFIED BY 'kurendo';
mysql> GRANT ALL PRIVILEGES ON * . * TO 'kurendo'@'localhost';
mysql> FLUSH PRIVILEGES;
```

* Create database and tables (copy paste won't work, please copy line by line):

```
mysql> CREATE DATABASE party_organizer;
mysql> USE party_organizer;
mysql> CREATE TABLE users (
		username VARCHAR(20) NOT NULL,
		password VARCHAR(120) NOT NULL,
		alias VARCHAR(20) DEFAULT NULL,
		PRIMARY KEY (username)
		);
mysql> INSERT INTO users VALUES
		("pesho", "123", "P3$h0"),
		("ice", "!11", "Icyy"),
		("mob", "uno", "Due");
```
* Add ```-lmysqlclient``` to the linker

