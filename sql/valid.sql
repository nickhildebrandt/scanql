-- SELECT
SELECT name, age FROM users WHERE id = 1;
SELECT * FROM products;
SELECT a, b, c, d FROM t WHERE id = 99;
SELECT a, b FROM orders WHERE status = 'active' AND total = 99;
SELECT * FROM logs WHERE level = 'error' OR level = 'warn';
SELECT * FROM t WHERE a = 1 OR b = 2 OR c = 3;
SELECT a FROM t WHERE x = 'v' AND y = 1 AND z = 2;
SELECT name FROM users WHERE name = 'alice';
SELECT id FROM t;
-- SELECT with double-quoted string
SELECT * FROM t WHERE x = "quoted";
-- SELECT zero literal
SELECT a FROM t WHERE id = 0;
-- SELECT with exact single result bound
SELECT * FROM t WHERE id = 1;
-- SELECT with JOIN chain
SELECT col FROM table1 JOIN table2 WHERE col = 1;
SELECT a FROM t JOIN b JOIN c WHERE a = 1;
SELECT a FROM t JOIN other WHERE a = 1;
-- INSERT
INSERT INTO users VALUES ('nick');
INSERT INTO logs VALUES ('startup', 0);
INSERT INTO orders VALUES (1, 'pending', 50);
INSERT INTO items VALUES ('book', 12);
-- INSERT with many values
INSERT INTO t VALUES ('a', 'b', 'c', 1, 2, 3);
-- INSERT single numeric value
INSERT INTO t VALUES (0);
-- DELETE
DELETE FROM sessions WHERE id = 42;
DELETE FROM logs WHERE level = 'debug';
DELETE FROM sessions WHERE token = 'abc';
-- DELETE with OR condition
DELETE FROM t WHERE a = 'x' OR b = 'y';
-- UPDATE
UPDATE users SET name = 'nick';
UPDATE items SET price = 99 WHERE id = 5;
UPDATE users SET age = 30 WHERE id = 1;
UPDATE orders SET status = 'done' WHERE id = 2;
-- UPDATE with multi-condition WHERE
UPDATE t SET a = 'x' WHERE b = 'y' AND c = 1;
