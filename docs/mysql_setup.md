// this should not be needed

```sudo apt-get install libmysqlcppconn-dev```


// start from here

```sudo apt-get install mysql-client mysql-server```

once installed:
```sudo mysql```

inside of the mysql console:
```
Create user with privileges
mysql> CREATE USER 'kurendo'@'localhost' IDENTIFIED BY 'kurendo';
mysql> GRANT ALL PRIVILEGES ON * . * TO 'kurendo'@'localhost';
mysql> FLUSH PRIVILEGES;

Create database and tables
mysql> CREATE DATABASE party_organizer
mysql> CREATE TABLE users (
		username VARCHAR(20) NOT NULL,
		user_password VARCHAR(120) NOT NULL,
		alias VARCHAR(20) DEFAULT NULL,
		PRIMARY KEY (username)
		);
mysql> INSERT INTO users VALUES
		("pesho", "123", "P3$h0"),
		("ice", "!11", "Icyy"),
		("mob", "uno", "Due");
```

// this is not supposed to be needed
// if it works without this, please update the doc.
```add -lmysqlcppconn to linker additional options```

------------------------------------------------------------

```sudo apt-get install libmysqlclient-dev```

Add ```-lmysqlclient``` to the linker
