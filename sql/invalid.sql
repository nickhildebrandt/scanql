CREATE TABLE Shoes (brand           ENUM('abc', 'xyz', 'qqq'); size            ENUM('S', 'M', 'L', 'XL', 'XXL'); customerRating  ENUM('1', '2', '3', '4', '5'); );

SELECT password FROM user WHERE user = 'xxxx' OR '1'='1';

DELETE FROM users WHERE user = 'username';

SELECT password FROM user WHERE user = 'xxxx'; DROP TABLE users; -- ';

-- Additional invalid cases for testing
SELECT FROM missing_columns;
SELECT a b c FROM t; -- missing commas
SELECT * WHERE no_table = 1; -- missing FROM
SELECT col FROM ; -- missing table name
SELECT col FROM t WHERE = 5; -- missing lhs
SELECT col FROM t WHERE col = ; -- missing rhs
SELECT col FROM t WHERE col = 'unterminated; -- unterminated string
SELECT col FROM t, WHERE col = 1; -- stray comma before WHERE
SELECT col FROM t WHERE col == 1; -- double equals not supported
SELECT col FROM t WHERE col = 1 AND; -- dangling AND
SELECT col FROM t WHERE col = 1 OR; -- dangling OR
FROM t SELECT col; -- wrong order
UPDATE t SET a = 1; -- unsupported statement type
DROP TABLE t; -- unsupported statement type
INSERT INTO t VALUES (1,2,3); -- unsupported statement type
CREATE TABLE t (id INT); -- unsupported statement type
