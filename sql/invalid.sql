-- Wrong start token
FROM users WHERE id = 1;
-- SELECT missing column
SELECT FROM t;
SELECT;
-- SELECT missing FROM keyword (second identifier after column)
SELECT a t;
-- WHERE missing LHS identifier
SELECT a FROM t WHERE = 1;
-- WHERE condition missing RHS value
SELECT a FROM t WHERE id = ;
-- WHERE AND without prior condition
SELECT a FROM t WHERE AND id = 1;
-- Two conditions without AND/OR connector
SELECT a FROM t WHERE id = 1 id = 2;
-- INSERT missing INTO
INSERT users VALUES ('nick');
-- DELETE missing FROM
DELETE WHERE id = 1;
-- Trailing comma before FROM
SELECT a, FROM t;
-- DELETE FROM with no table name
DELETE FROM;
-- INSERT INTO with no table name
INSERT INTO;
-- AND/OR with no right-hand side
SELECT a FROM t WHERE id = 1 AND;
SELECT a FROM t WHERE id = 1 OR;
-- VALUES with nothing (no parens)
INSERT INTO t VALUES;
-- VALUES with empty parens
INSERT INTO t VALUES ();
-- VALUES with only a comma inside
INSERT INTO t VALUES (,);
-- SET with missing RHS
UPDATE t SET a = ;
-- SET with missing LHS identifier
UPDATE t SET = 'x';
-- WHERE with nothing after it
DELETE FROM t WHERE ;
-- Trailing comma directly before FROM
SELECT a, b, FROM t;
-- FROM followed by FROM keyword
SELECT FROM FROM t;
-- AND followed by a bare number (not an identifier)
SELECT a FROM t WHERE id = 1 AND 2;
-- = without spaces: missing LHS identifier
SELECT a FROM t WHERE =1;
-- = without spaces: SET missing LHS identifier
UPDATE t SET =1;
-- = without spaces: missing RHS value
SELECT a FROM t WHERE id= ;
