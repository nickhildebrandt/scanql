SELECT user, host FROM user;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `eastwind` /*!40100 DEFAULT CHARACTER SET latin1 */;

SELECT name WHERE dateOfBirth BETWEEN @ali_dob AND @kumar_dob;

CREATE TABLE IF NOT EXISTS `blob_arena` (`id`     INT UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT, `photo`  BLOB,PRIMARY KEY(`ID`) );

CREATE TABLE IF NOT EXISTS products (
   productID    INT UNSIGNED  NOT NULL AUTO_INCREMENT,
   productCode  CHAR(3)       NOT NULL DEFAULT '',
   name         VARCHAR(30)   NOT NULL DEFAULT '',
   quantity     INT UNSIGNED  NOT NULL DEFAULT 0,
   price        DECIMAL(7,2)  NOT NULL DEFAULT 99999.99,
   PRIMARY KEY  (productID)
);

INSERT INTO products VALUES
   (1001, 'PEN', 'Pen Red',    5000, 1.23),
   (1002, 'PEN', 'Pen Blue',   8000, 1.25),
   (1003, 'PEN', 'Pen Black',  2000, 1.25),
   (1004, 'PEC', 'Pencil 2B', 10000, 0.48),
   (1005, 'PEC', 'Pencil 2H',  8000, 0.49);

SELECT * FROM `products`;

CREATE TABLE shop (
    article INT UNSIGNED  DEFAULT '0000' NOT NULL,
    dealer  CHAR(20)      DEFAULT ''     NOT NULL,
    price   DECIMAL(16,2) DEFAULT '0.00' NOT NULL,
    PRIMARY KEY(article, dealer));

INSERT INTO shop VALUES
    (1,'A',3.45),(1,'B',3.99),(2,'A',10.99),(3,'B',1.45),
    (3,'C',1.69),(3,'D',1.25),(4,'D',19.95);

-- Additional simple SELECTs that fit the minimal validator subset
SELECT id, name FROM customers WHERE id = 1;
SELECT * FROM users WHERE age = 30 AND city = 'Berlin';
SELECT a, b FROM t WHERE a = 1 OR b = 2;
SELECT col FROM tbl WHERE col = 123;

-- Star and multiple columns
SELECT *, col1 FROM demo WHERE col2 = 'x';
SELECT col1, col2, col3 FROM demo_table WHERE col3 = 42;

-- Chained AND/OR
SELECT x FROM foo WHERE a = 1 AND b = 2 OR c = 3;
SELECT user FROM accounts WHERE active = 1 AND suspended = 0;

-- Trailing semicolon termination
SELECT value FROM config WHERE key = 'site_name';
SELECT user, host FROM user;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `eastwind` /*!40100 DEFAULT CHARACTER SET latin1 */;

SELECT name WHERE dateOfBirth BETWEEN @ali_dob AND @kumar_dob;

CREATE TABLE IF NOT EXISTS `blob_arena` (`id`     INT UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT, `photo`  BLOB,PRIMARY KEY(`ID`) );

CREATE TABLE Shoes (brand           ENUM('abc', 'xyz', 'qqq'); size            ENUM('S', 'M', 'L', 'XL', 'XXL'); customerRating  ENUM('1', '2', '3', '4', '5'); );

SELECT password FROM user WHERE user = 'xxxx' OR '1'='1';

DELETE FROM users WHERE user = 'username';

SELECT password FROM user WHERE user = 'xxxx'; DROP TABLE users; -- ';

CREATE TABLE IF NOT EXISTS products (
   productID    INT UNSIGNED  NOT NULL AUTO_INCREMENT,
   productCode  CHAR(3)       NOT NULL DEFAULT '',
   name         VARCHAR(30)   NOT NULL DEFAULT '',
   quantity     INT UNSIGNED  NOT NULL DEFAULT 0,
   price        DECIMAL(7,2)  NOT NULL DEFAULT 99999.99,
   PRIMARY KEY  (productID)
);

INSERT INTO products VALUES
   (1001, 'PEN', 'Pen Red',    5000, 1.23),
   (1002, 'PEN', 'Pen Blue',   8000, 1.25),
   (1003, 'PEN', 'Pen Black',  2000, 1.25),
   (1004, 'PEC', 'Pencil 2B', 10000, 0.48),
   (1005, 'PEC', 'Pencil 2H',  8000, 0.49);
SELECT * FROM `products`;

CREATE TABLE shop (
    article INT UNSIGNED  DEFAULT '0000' NOT NULL,
    dealer  CHAR(20)      DEFAULT ''     NOT NULL,
    price   DECIMAL(16,2) DEFAULT '0.00' NOT NULL,
    PRIMARY KEY(article, dealer));
INSERT INTO shop VALUES
    (1,'A',3.45),(1,'B',3.99),(2,'A',10.99),(3,'B',1.45),
    (3,'C',1.69),(3,'D',1.25),(4,'D',19.95);
