-- SELECT: basic
SELECT a FROM t;
SELECT name FROM users;
SELECT id FROM t;
SELECT a FROM t WHERE id = 1;
SELECT * FROM t;
SELECT * FROM products;
SELECT a, b FROM t;
SELECT a, b, c FROM t;
SELECT a, b, c, d FROM t;
SELECT a, b, c, d, e FROM t;
SELECT name, age FROM users WHERE id = 1;
SELECT a, b, c, d FROM t WHERE id = 99;
-- SELECT: WHERE with string values
SELECT name FROM users WHERE name = 'alice';
SELECT * FROM t WHERE x = "quoted";
SELECT a FROM t WHERE status = 'active';
SELECT a FROM t WHERE status = "active";
SELECT a FROM t WHERE name = 'O';
SELECT a FROM t WHERE col = '123';
SELECT a FROM t WHERE col = "hello world";
-- SELECT: WHERE with numbers
SELECT a FROM t WHERE id = 0;
SELECT a FROM t WHERE id = 1;
SELECT a FROM t WHERE id = 42;
SELECT a FROM t WHERE id = 999;
SELECT a FROM t WHERE id = 100000;
-- SELECT: WHERE with AND
SELECT a, b FROM orders WHERE status = 'active' AND total = 99;
SELECT a FROM t WHERE x = 'v' AND y = 1 AND z = 2;
SELECT a FROM t WHERE a = 1 AND b = 2;
SELECT a FROM t WHERE a = 1 AND b = 2 AND c = 3;
SELECT a FROM t WHERE a = 1 AND b = 2 AND c = 3 AND d = 4;
SELECT a FROM t WHERE x = 'a' AND y = 'b' AND z = 'c';
-- SELECT: WHERE with OR
SELECT * FROM logs WHERE level = 'error' OR level = 'warn';
SELECT * FROM t WHERE a = 1 OR b = 2 OR c = 3;
SELECT a FROM t WHERE id = 1 OR id = 2;
SELECT a FROM t WHERE id = 1 OR id = 2 OR id = 3;
SELECT a FROM t WHERE name = 'a' OR name = 'b';
-- SELECT: WHERE with AND and OR mixed
SELECT a FROM t WHERE a = 1 AND b = 2 OR c = 3;
SELECT a FROM t WHERE a = 1 OR b = 2 AND c = 3;
SELECT a FROM t WHERE a = 1 AND b = 2 AND c = 3 OR d = 4;
SELECT a FROM t WHERE a = 1 OR b = 2 OR c = 3 AND d = 4;
-- SELECT: with star
SELECT * FROM t WHERE id = 1;
SELECT * FROM t WHERE a = 1 AND b = 2;
SELECT * FROM t WHERE a = 1 OR b = 2;
-- SELECT: star without WHERE
SELECT * FROM t;
SELECT * FROM users;
SELECT * FROM products;
SELECT * FROM logs;
-- SELECT: without WHERE
SELECT a FROM t;
SELECT a, b FROM t;
SELECT a, b, c FROM t;
SELECT name FROM users;
SELECT id, name FROM users;
-- SELECT: with JOIN
SELECT col FROM table1 JOIN table2 WHERE col = 1;
SELECT a FROM t JOIN b JOIN c WHERE a = 1;
SELECT a FROM t JOIN other WHERE a = 1;
SELECT a FROM t JOIN b WHERE a = 1;
SELECT a, b FROM t JOIN u WHERE t = 1;
SELECT * FROM t JOIN u WHERE id = 1;
SELECT a FROM t JOIN b JOIN c JOIN d WHERE a = 1;
SELECT a FROM x JOIN y WHERE x = 'val';
-- SELECT: without semicolon
SELECT a FROM t
SELECT * FROM t
SELECT a FROM t WHERE id = 1
SELECT a, b FROM t WHERE a = 1 AND b = 2
-- SELECT: case variations
select a from t;
SELECT A FROM T;
Select Name From Users;
sElEcT a FrOm t;
select * from t where id = 1;
Select A From T Where Id = 1;
-- SELECT: equals without spaces
SELECT a FROM t WHERE id=1;
SELECT a FROM t WHERE id=42;
SELECT a FROM t WHERE name='alice';
SELECT a FROM t WHERE name="bob";
SELECT a, b FROM t WHERE a=1 AND b=2;
SELECT * FROM t WHERE a=1 OR b=2;
-- SELECT: many columns
SELECT a, b, c, d, e, f FROM t;
SELECT a, b, c, d, e, f, g FROM t;
SELECT a, b, c, d, e, f, g, h FROM t;
SELECT a, b, c, d, e, f, g, h, i FROM t;
SELECT a, b, c, d, e, f, g, h, i, j FROM t;
SELECT a, b, c, d, e, f, g, h, i, j, k, l FROM t;
SELECT a, b, c, d, e, f, g, h, i, j, k, l, m, n, o FROM t;
-- SELECT: various WHERE comparisons
SELECT a FROM t WHERE col = 'value';
SELECT a FROM t WHERE col = "value";
SELECT a FROM t WHERE col = 0;
SELECT a FROM t WHERE col = 123;
SELECT a FROM t WHERE col = abc;
-- SELECT: additional patterns
SELECT a FROM t WHERE id = 1 AND name = 'x' AND age = 30;
SELECT a FROM t WHERE id = 1 OR name = 'x' OR age = 30;
SELECT a, b FROM t JOIN u WHERE a = 1 AND b = 2;
SELECT * FROM t JOIN u WHERE a = 'x';
SELECT a FROM t WHERE a = a;
SELECT * FROM t WHERE col = col2;
SELECT *, a FROM t;
SELECT a, b, c FROM t WHERE x = 1;
SELECT a, b, c FROM t WHERE x = 1 AND y = 2;
SELECT a, b, c FROM t WHERE x = 1 OR y = 2;
SELECT id FROM users WHERE name = 'admin';
-- SELECT: long identifiers
SELECT abcdefghijklmnopqrstuvwxyz FROM abcdefghijklmnopqrstuvwxyz;
SELECT my_col FROM my_table WHERE my_id = 1;
SELECT _a FROM _t WHERE _id = 1;
SELECT a_b_c FROM t_u_v WHERE x_y_z = 1;
SELECT col1 FROM table2 WHERE id3 = 4;
-- SELECT: multiple JOINs
SELECT a FROM t JOIN u JOIN v JOIN w WHERE a = 1;
SELECT a FROM t JOIN u JOIN v JOIN w JOIN x WHERE a = 1;
-- SELECT: AND/OR chains
SELECT a FROM t WHERE a = 1 AND b = 2 AND c = 3 AND d = 4 AND e = 5;
SELECT a FROM t WHERE a = 1 OR b = 2 OR c = 3 OR d = 4 OR e = 5;
SELECT a FROM t WHERE a = 1 AND b = 2 AND c = 3 AND d = 4 AND e = 5 AND f = 6;
SELECT a FROM t WHERE a = 1 OR b = 2 OR c = 3 OR d = 4 OR e = 5 OR f = 6;
-- SELECT: edge cases
SELECT a FROM t WHERE a = 0 AND b = 0;
SELECT a FROM t WHERE a = 0 OR b = 0;
-- INSERT: basic
INSERT INTO users VALUES ('nick');
INSERT INTO logs VALUES ('startup', 0);
INSERT INTO orders VALUES (1, 'pending', 50);
INSERT INTO items VALUES ('book', 12);
INSERT INTO t VALUES ('a', 'b', 'c', 1, 2, 3);
INSERT INTO t VALUES (0);
INSERT INTO t VALUES (1);
INSERT INTO t VALUES (42);
INSERT INTO t VALUES ('hello');
INSERT INTO t VALUES ("hello");
INSERT INTO t VALUES (1, 2, 3);
INSERT INTO t VALUES ('a', 'b', 'c');
INSERT INTO t VALUES (1, 'two', 3);
INSERT INTO t VALUES ('a', 2, 'c', 4);
INSERT INTO t VALUES (100, 200, 300, 400, 500);
INSERT INTO t VALUES ('x');
INSERT INTO t VALUES ("x");
INSERT INTO users VALUES ('alice');
INSERT INTO users VALUES ('bob');
INSERT INTO users VALUES ('charlie');
INSERT INTO products VALUES ('widget', 10);
INSERT INTO products VALUES ('gadget', 20);
INSERT INTO logs VALUES ('info', 1);
INSERT INTO logs VALUES ('warn', 2);
INSERT INTO logs VALUES ('error', 3);
INSERT INTO audit VALUES ('login', 1);
-- INSERT: case variations
insert into t values (1);
INSERT INTO T VALUES (1);
Insert Into Users Values ('nick');
insert into t values ('a', 'b');
INSERT INTO t VALUES ('A', 'B');
-- INSERT: without semicolon
INSERT INTO t VALUES (1)
INSERT INTO t VALUES ('hello')
INSERT INTO t VALUES (1, 2, 3)
-- INSERT: many values
INSERT INTO t VALUES (1, 2, 3, 4, 5, 6, 7, 8);
INSERT INTO t VALUES ('a', 'b', 'c', 'd', 'e', 'f');
INSERT INTO t VALUES (1, 'a', 2, 'b', 3, 'c', 4, 'd');
INSERT INTO t VALUES (0, 0, 0, 0, 0);
INSERT INTO t VALUES ('x', 'y', 'z');
INSERT INTO t VALUES (1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
INSERT INTO t VALUES ('a', 'b', 'c', 'd', 'e', 'f', 'g', 'h');
-- INSERT: mixed value types
INSERT INTO t VALUES (1, 'a', "b", 0, 'c', "d", 42);
INSERT INTO t VALUES ("hello", "world");
INSERT INTO t VALUES ("a", "b", "c");
INSERT INTO t VALUES (1, 2);
INSERT INTO t VALUES ('a', 1);
INSERT INTO t VALUES (1, 'a');
INSERT INTO t VALUES ("a", 1);
-- DELETE: basic
DELETE FROM sessions WHERE id = 42;
DELETE FROM logs WHERE level = 'debug';
DELETE FROM sessions WHERE token = 'abc';
DELETE FROM t WHERE a = 'x' OR b = 'y';
DELETE FROM t WHERE id = 1;
DELETE FROM t WHERE id = 0;
DELETE FROM t WHERE name = 'alice';
DELETE FROM t WHERE name = "bob";
DELETE FROM t WHERE a = 1 AND b = 2;
DELETE FROM t WHERE a = 1 OR b = 2;
DELETE FROM t WHERE a = 1 AND b = 2 AND c = 3;
DELETE FROM t WHERE a = 1 OR b = 2 OR c = 3;
DELETE FROM t WHERE a = 1 AND b = 'x';
DELETE FROM users WHERE id = 1;
DELETE FROM users WHERE name = 'nick';
DELETE FROM logs WHERE level = 'info';
DELETE FROM products WHERE price = 0;
DELETE FROM orders WHERE status = 'cancelled';
DELETE FROM cache WHERE expired = 1;
DELETE FROM t WHERE a = 'x' AND b = 1;
DELETE FROM t WHERE a = "x" OR b = 0;
-- DELETE: case variations
delete from t where id = 1;
DELETE FROM T WHERE ID = 1;
Delete From Users Where Name = 'alice';
-- DELETE: without semicolon
DELETE FROM t WHERE id = 1
DELETE FROM t WHERE name = 'x'
DELETE FROM t WHERE a = 1 AND b = 2
-- DELETE: equals without spaces
DELETE FROM t WHERE id=42;
DELETE FROM t WHERE name='alice';
DELETE FROM t WHERE id=1 AND name='bob';
-- DELETE: additional
DELETE FROM t WHERE id = 99;
DELETE FROM t WHERE a = 'val' AND b = 'val2';
DELETE FROM t WHERE a = 1 AND b = 2 AND c = 3;
DELETE FROM t WHERE id = 0;
DELETE FROM t WHERE a = 'val' OR b = 'val2';
DELETE FROM t WHERE a = 1 AND b = 'x' AND c = 3;
DELETE FROM t WHERE x = y;
DELETE FROM t WHERE a = 'long string value';
-- UPDATE: basic
UPDATE users SET name = 'nick';
UPDATE items SET price = 99 WHERE id = 5;
UPDATE users SET age = 30 WHERE id = 1;
UPDATE orders SET status = 'done' WHERE id = 2;
UPDATE t SET a = 'x' WHERE b = 'y' AND c = 1;
UPDATE t SET a = 1;
UPDATE t SET a = 0;
UPDATE t SET col = 'value';
UPDATE t SET col = "value";
UPDATE t SET col = 42;
UPDATE t SET col = 999;
UPDATE t SET name = 'alice' WHERE id = 1;
UPDATE t SET name = 'bob' WHERE id = 2;
UPDATE t SET price = 10 WHERE category = 'books';
UPDATE t SET status = 'active' WHERE id = 1 AND type = 'user';
UPDATE t SET status = 'inactive' WHERE id = 1 OR id = 2;
UPDATE t SET a = 1 WHERE b = 2 AND c = 3 AND d = 4;
UPDATE users SET email = 'a' WHERE id = 1;
UPDATE products SET stock = 0 WHERE id = 99;
UPDATE logs SET level = 'error' WHERE id = 1;
UPDATE settings SET value = 'dark' WHERE key = 'theme';
UPDATE t SET name = 'nick' WHERE id = 1;
UPDATE t SET name = "nick" WHERE id = 1;
UPDATE t SET count = 0 WHERE id = 1;
UPDATE t SET a = b WHERE c = d;
UPDATE t SET col = 100;
UPDATE t SET col = 'updated';
UPDATE t SET col = "updated";
UPDATE t SET col = 0 WHERE id = 1;
-- UPDATE: case variations
update t set a = 1;
UPDATE T SET A = 1;
Update Users Set Name = 'nick';
update t set a = 1 where id = 1;
-- UPDATE: without semicolon
UPDATE t SET a = 1
UPDATE t SET a = 1 WHERE id = 1
UPDATE t SET col = 'value' WHERE id = 42
-- UPDATE: equals without spaces
UPDATE t SET name='nick' WHERE id=1;
UPDATE t SET a=1;
UPDATE t SET a=1 WHERE b=2;
UPDATE t SET a='x' WHERE b='y' AND c=1;
-- UPDATE: additional
UPDATE t SET x = 'hello' WHERE a = 1 AND b = 2;
UPDATE t SET x = 123 WHERE y = 456;
UPDATE t SET x = y WHERE a = b;
UPDATE t SET a = 1 WHERE b = 2 OR c = 3;
-- CREATE TABLE: basic
CREATE TABLE t (id INTEGER);
CREATE TABLE users (id INTEGER, name TEXT);
CREATE TABLE users (id INTEGER, name TEXT, age INTEGER);
CREATE TABLE products (id INTEGER, name TEXT, price INTEGER, stock INTEGER);
CREATE TABLE logs (id INTEGER, level TEXT, message TEXT, timestamp INTEGER);
CREATE TABLE t (a INTEGER, b TEXT, c INTEGER, d TEXT, e INTEGER);
CREATE TABLE orders (id INTEGER, user_id INTEGER, status TEXT, total INTEGER);
CREATE TABLE sessions (id INTEGER, token TEXT, user_id INTEGER);
CREATE TABLE items (id INTEGER, name TEXT, price INTEGER);
CREATE TABLE events (id INTEGER, type TEXT, data TEXT);
CREATE TABLE config (key TEXT, value TEXT);
CREATE TABLE migrations (id INTEGER, name TEXT, applied INTEGER);
CREATE TABLE t (col1 INTEGER, col2 TEXT, col3 INTEGER, col4 TEXT, col5 INTEGER, col6 TEXT);
-- CREATE TABLE: case variations
create table t (id integer);
CREATE TABLE T (ID INTEGER);
Create Table Users (Id Integer, Name Text);
create table t (a integer, b text);
-- CREATE TABLE: without semicolon
CREATE TABLE t (id INTEGER)
CREATE TABLE users (id INTEGER, name TEXT)
CREATE TABLE t (a INTEGER, b TEXT, c INTEGER)
-- CREATE TABLE: single column various types
CREATE TABLE t (name TEXT);
CREATE TABLE t (val REAL);
CREATE TABLE t (flag BOOLEAN);
CREATE TABLE t (data BLOB);
CREATE TABLE t (ts TIMESTAMP);
CREATE TABLE t (a VARCHAR);
CREATE TABLE t (a BIGINT);
CREATE TABLE t (a FLOAT);
CREATE TABLE t (id SERIAL);
-- CREATE TABLE: many columns
CREATE TABLE t (a INT, b INT, c INT, d INT, e INT, f INT, g INT, h INT);
CREATE TABLE t (c1 TEXT, c2 TEXT, c3 TEXT, c4 TEXT, c5 TEXT);
CREATE TABLE t (x INTEGER, y INTEGER, z INTEGER);
CREATE TABLE large (c1 INT, c2 INT, c3 INT, c4 INT, c5 INT, c6 INT, c7 INT, c8 INT, c9 INT, c10 INT);
CREATE TABLE t (a A, b B, c C, d D, e E, f F, g G, h H);
-- CREATE TABLE: various type names
CREATE TABLE t (a CHAR, b SMALLINT);
CREATE TABLE t (a DOUBLE, b NUMERIC);
CREATE TABLE t (id SERIAL, name VARCHAR, age INT);
CREATE TABLE z (z Z);
CREATE TABLE t2 (id INT, val TEXT);
CREATE TABLE my_table (col1 INT, col2 TEXT, col3 INT);
CREATE TABLE _t (_id INT, _name TEXT);
CREATE TABLE abc (x INT);
CREATE TABLE tbl (a INT, b INT, c INT, d INT, e INT);
-- Mixed case stress test
SeLeCt a FrOm t WhErE iD = 1;
InSeRt InTo t VaLuEs ('x');
DeLeTe FrOm t WhErE iD = 1;
UpDaTe t SeT a = 1;
CrEaTe TaBlE t (iD iNt);
sElEcT a fRoM t wHeRe id = 1;
-- Extra whitespace
SELECT  a  FROM  t  WHERE  id  =  1;
SELECT a FROM t;
-- JOIN with various conditions
SELECT a FROM t JOIN u WHERE id = 1 AND name = 'x';
SELECT a FROM t JOIN u WHERE id = 1 OR name = 'x';
SELECT a, b FROM t JOIN u JOIN v WHERE a = 1;
SELECT * FROM t JOIN u JOIN v WHERE x = 'y';
SELECT a FROM t JOIN u WHERE a = 1 AND b = 2 AND c = 3;
-- Identifier as RHS of equals
SELECT a FROM t WHERE col = other;
SELECT a FROM t WHERE col = other AND b = c;
SELECT a FROM t WHERE a = b OR c = d;
-- Additional INSERT
INSERT INTO t VALUES ('test');
INSERT INTO t VALUES ("test");
INSERT INTO t VALUES (99);
INSERT INTO t VALUES (1, 2, 3, 4, 5);
INSERT INTO t VALUES ('a', 'b', 'c', 'd');
