-- Wrong start token
FROM users WHERE id = 1;
WHERE id = 1;
AND id = 1;
OR id = 1;
INTO t VALUES (1);
VALUES (1);
SET a = 1;
JOIN t WHERE a = 1;
TABLE t (id INT);
,;
=;
*;
(1);
);
1;
'hello';
"hello";
-- SELECT errors: missing column
SELECT FROM t;
SELECT;
-- SELECT: missing FROM keyword
SELECT a t;
SELECT a b;
SELECT a b c;
-- SELECT: wrong token after column
-- SELECT: comma then no next column
SELECT a, FROM t;
SELECT a, b, FROM t;
SELECT a, b, c, FROM t;
-- SELECT: no table after FROM
SELECT a FROM;
SELECT * FROM;
-- SELECT: WHERE but no condition
SELECT a FROM t WHERE;
SELECT * FROM t WHERE;
-- WHERE: missing LHS identifier
SELECT a FROM t WHERE = 1;
SELECT * FROM t WHERE = 'x';
-- WHERE: missing RHS value
SELECT a FROM t WHERE id = ;
SELECT a FROM t WHERE id =;
SELECT a FROM t WHERE name = ;
-- WHERE: AND without prior condition
SELECT a FROM t WHERE AND id = 1;
SELECT a FROM t WHERE OR id = 1;
-- Two conditions without AND/OR
SELECT a FROM t WHERE id = 1 id = 2;
SELECT a FROM t WHERE id = 1 name = 'x';
-- AND/OR with no right-hand side
SELECT a FROM t WHERE id = 1 AND;
SELECT a FROM t WHERE id = 1 OR;
SELECT a FROM t WHERE a = 1 AND b = 2 AND;
SELECT a FROM t WHERE a = 1 OR b = 2 OR;
-- AND/OR followed by wrong token
SELECT a FROM t WHERE id = 1 AND 2;
SELECT a FROM t WHERE id = 1 AND 'x';
SELECT a FROM t WHERE id = 1 OR 2;
SELECT a FROM t WHERE id = 1 OR 'x';
-- FROM followed by keyword
SELECT a FROM WHERE id = 1;
SELECT a FROM SELECT;
SELECT a FROM INSERT;
SELECT a FROM DELETE;
SELECT FROM FROM t;
-- Bare star not after SELECT
SELECT a FROM t WHERE * = 1;
-- Extra comma at start
SELECT , a FROM t;
-- Semicolon in wrong position
SELECT ; a FROM t;
-- Just semicolon
;
-- INSERT errors: missing INTO
INSERT users VALUES ('nick');
INSERT VALUES ('nick');
INSERT t VALUES ('nick');
-- INSERT INTO: no table
INSERT INTO;
-- INSERT INTO table: no VALUES
-- VALUES: with nothing
INSERT INTO t VALUES;
-- VALUES: empty parens
INSERT INTO t VALUES ();
-- VALUES: only a comma
INSERT INTO t VALUES (,);
INSERT INTO t VALUES (, ,);
-- VALUES: missing close paren
-- VALUES: no open paren
INSERT INTO t VALUES 1;
INSERT INTO t VALUES 'a';
-- INSERT INTO INTO
INSERT INTO INTO VALUES (1);
-- INSERT: VALUES keyword repeated
INSERT INTO t VALUES VALUES (1);
-- INSERT: wrong token after table
-- VALUES: wrong token after comma inside parens
INSERT INTO t VALUES (1, );
INSERT INTO t VALUES (1, , 2);
INSERT INTO t VALUES (, 1);
-- DELETE errors: missing FROM
DELETE WHERE id = 1;
DELETE t WHERE id = 1;
DELETE;
-- DELETE FROM: no table
DELETE FROM;
-- DELETE FROM: missing WHERE keyword
DELETE FROM t id = 1;
DELETE FROM t a = 1;
-- DELETE FROM WHERE: missing table
DELETE FROM WHERE id = 1;
-- DELETE: double FROM
DELETE FROM FROM t WHERE id = 1;
-- DELETE FROM t WHERE: no condition
DELETE FROM t WHERE;
-- DELETE: incomplete condition
DELETE FROM t WHERE id =;
DELETE FROM t WHERE id = ;
-- DELETE: wrong token after table
-- DELETE: dangling AND/OR
DELETE FROM t WHERE id = 1 AND;
DELETE FROM t WHERE id = 1 OR;
-- UPDATE errors: missing table
UPDATE;
UPDATE SET a = 1;
-- UPDATE: missing SET
UPDATE t a = 1;
-- UPDATE SET: missing LHS
UPDATE t SET = 'x';
UPDATE t SET = 1;
-- UPDATE SET: missing RHS
UPDATE t SET a = ;
UPDATE t SET a =;
-- UPDATE SET: missing equals
UPDATE t SET a 1;
UPDATE t SET a 'x';
-- UPDATE: double SET
UPDATE t SET a = 1 SET b = 2;
-- UPDATE: dangling AND/OR
UPDATE t SET a = 1 WHERE id = 1 AND;
UPDATE t SET a = 1 WHERE id = 1 OR;
-- UPDATE: missing WHERE condition
UPDATE t SET a = 1 WHERE;
-- UPDATE: wrong token after table
-- UPDATE SET: followed by keyword
UPDATE t SET FROM;
UPDATE t SET WHERE;
UPDATE t SET SELECT;
-- CREATE TABLE errors: missing TABLE
CREATE t (id INTEGER);
CREATE (id INTEGER);
CREATE;
-- CREATE TABLE: missing table name
CREATE TABLE;
-- CREATE TABLE: missing parens
CREATE TABLE t;
-- CREATE TABLE: empty parens
CREATE TABLE t ();
-- CREATE TABLE: missing column type
CREATE TABLE t (id);
CREATE TABLE t (id, name TEXT);
CREATE TABLE t (id INTEGER, name);
-- CREATE TABLE: extra comma
CREATE TABLE t (id INTEGER,);
CREATE TABLE t (id INTEGER, name TEXT,);
-- CREATE TABLE: leading comma
CREATE TABLE t (, id INTEGER);
-- CREATE TABLE: double comma
CREATE TABLE t (id INTEGER,, name TEXT);
-- CREATE TABLE: missing close paren
CREATE TABLE t (id INTEGER;
CREATE TABLE t (id INTEGER, name TEXT;
-- CREATE TABLE: wrong token inside
CREATE TABLE t (id INTEGER AND name TEXT);
CREATE TABLE t (id = INTEGER);
CREATE TABLE t (id INTEGER OR name TEXT);
-- CREATE TABLE: nested parens
CREATE TABLE t ((id INTEGER));
-- CREATE TABLE: semicolon inside parens
CREATE TABLE t (id INTEGER; name TEXT);
-- CREATE TABLE: VALUES after name
CREATE TABLE t VALUES (1);
-- CREATE TABLE: wrong keyword after name
CREATE TABLE t FROM u;
CREATE TABLE t WHERE a = 1;
CREATE TABLE t SET a = 1;
-- = without spaces errors
SELECT a FROM t WHERE =1;
UPDATE t SET =1;
SELECT a FROM t WHERE id= ;
UPDATE t SET a= ;
DELETE FROM t WHERE =1;
DELETE FROM t WHERE id= ;
-- Case variation: still invalid
select from t;
SELECT FROM T;
Select From T;
insert users values ('nick');
INSERT USERS VALUES ('nick');
delete where id = 1;
DELETE WHERE ID = 1;
update set a = 1;
UPDATE SET A = 1;
create t (id int);
CREATE T (ID INT);
-- Multiple errors in one statement
SELECT FROM;
SELECT , FROM t;
INSERT INTO;
INSERT INTO t VALUES;
DELETE FROM;
DELETE;
UPDATE;
UPDATE t SET;
CREATE TABLE;
CREATE TABLE t;
CREATE;
-- Just keywords
FROM;
WHERE;
AND;
OR;
INTO;
VALUES;
SET;
JOIN;
TABLE;
-- Double keywords
SELECT SELECT a FROM t;
INSERT INSERT INTO t VALUES (1);
DELETE DELETE FROM t WHERE id = 1;
UPDATE UPDATE t SET a = 1;
CREATE CREATE TABLE t (id INT);
-- Wrong keyword order
FROM SELECT a t;
WHERE SELECT a FROM t;
VALUES INSERT INTO t;
SET UPDATE t;
-- Start with punctuation
, SELECT a FROM t;
= SELECT a FROM t;
( SELECT a FROM t;
) SELECT a FROM t;
-- Start with literal
1 SELECT a FROM t;
'hello' SELECT a FROM t;
"hello" SELECT a FROM t;
-- Trailing junk after valid
SELECT a FROM t WHERE id = 1 extra;
INSERT INTO t VALUES (1) extra;
DELETE FROM t WHERE id = 1 extra;
UPDATE t SET a = 1 extra;
-- Wrong nesting
SELECT a FROM t WHERE (id = 1);
INSERT INTO t (VALUES (1));
-- More invalid SELECT
SELECT a FROM t WHERE id = 1 AND = 2;
SELECT a FROM t WHERE id = 1 OR = 2;
SELECT a FROM t WHERE AND = 1;
SELECT a FROM t WHERE = ;
SELECT a FROM t WHERE id = AND;
SELECT a FROM t WHERE id = OR;
SELECT a FROM t WHERE id = FROM;
SELECT a FROM t WHERE id = WHERE;
SELECT a FROM t WHERE id = SELECT;
-- Invalid tokens inside VALUES
INSERT INTO t VALUES (SELECT);
INSERT INTO t VALUES (FROM);
INSERT INTO t VALUES (WHERE);
INSERT INTO t VALUES (AND);
INSERT INTO t VALUES (OR);
INSERT INTO t VALUES (INSERT);
INSERT INTO t VALUES (DELETE);
INSERT INTO t VALUES (UPDATE);
INSERT INTO t VALUES (=);
INSERT INTO t VALUES (*);
-- More DELETE errors
DELETE FROM t WHERE = = 1;
-- More UPDATE errors
UPDATE t SET a = 1 WHERE id = 1 AND;
UPDATE t SET a = 1 WHERE id = 1 OR;
UPDATE t SET a = 1 WHERE = 1;
-- More CREATE TABLE errors
CREATE TABLE t (id INTEGER name TEXT);
CREATE TABLE t (,);
CREATE TABLE t (id,);
CREATE TABLE t (id INTEGER, ,);
CREATE TABLE t (1 INTEGER);
CREATE TABLE t (id 1);
CREATE TABLE t (id 'INTEGER');
-- Wrong statement transitions
SELECT a FROM t INSERT INTO u VALUES (1);
SELECT a FROM t DELETE FROM u WHERE id = 1;
SELECT a FROM t UPDATE u SET a = 1;
SELECT a FROM t CREATE TABLE u (id INT);
INSERT INTO t VALUES (1) SELECT a FROM u;
INSERT INTO t VALUES (1) DELETE FROM u;
DELETE FROM t WHERE id = 1 INSERT INTO u VALUES (1);
DELETE FROM t WHERE id = 1 SELECT a FROM u;
UPDATE t SET a = 1 SELECT b FROM u;
UPDATE t SET a = 1 INSERT INTO u VALUES (1);
-- Double semicolons (second ; starts a new empty statement with just ;)
SELECT a FROM t;;
INSERT INTO t VALUES (1);;
DELETE FROM t WHERE id = 1;;
UPDATE t SET a = 1;;
CREATE TABLE t (id INT);;
-- Mixed invalid keywords
FROM WHERE AND;
WHERE AND OR;
AND OR FROM;
OR AND WHERE;
INTO VALUES SET;
VALUES SET JOIN;
SET JOIN TABLE;
-- Additional SELECT errors
SELECT * WHERE id = 1;
SELECT * JOIN t;
SELECT * AND a FROM t;
SELECT * SET a = 1;
SELECT * VALUES (1);
SELECT * INSERT INTO t;
-- WHERE with keywords as values
SELECT a FROM t WHERE id = SET;
SELECT a FROM t WHERE id = VALUES;
-- INSERT wrong sequences
INSERT INTO t (1);
INSERT INTO t 1;
INSERT VALUES (1);
INSERT FROM t;
INSERT SET a = 1;
INSERT WHERE a = 1;
-- DELETE wrong keywords
DELETE INTO t WHERE id = 1;
DELETE SET a = 1;
DELETE VALUES (1);
DELETE JOIN t WHERE a = 1;
-- UPDATE wrong keywords
UPDATE t INTO u;
-- CREATE with wrong keywords
CREATE TABLE t SELECT a FROM u;
CREATE TABLE t DELETE FROM u;
CREATE TABLE t UPDATE u SET a = 1;
CREATE SELECT a FROM t;
CREATE DELETE FROM t;
CREATE UPDATE t SET a = 1;
CREATE INSERT INTO t VALUES (1);
-- Additional invalid edge cases
CREATE TABLE t (id INTEGER) extra;
CREATE TABLE t (id INTEGER) WHERE;
CREATE TABLE t (id INTEGER) FROM;
SELECT a FROM t WHERE id = 1 SET a = 2;
CREATE TABLE t (id INTEGER, 1 TEXT);
CREATE TABLE t (id INTEGER, 'name' TEXT);
CREATE TABLE t (id INTEGER, "name" TEXT);
SELECT FROM WHERE;
INSERT INTO VALUES;
DELETE FROM SET;
UPDATE WHERE SET;
SELECT a FROM t JOIN;
SELECT a FROM t JOIN WHERE id = 1;
-- Additional invalid cases to reach 300+
SELECT a FROM t WHERE id = 1 AND = ;
SELECT a FROM t WHERE id = 1 OR = ;
SELECT FROM t WHERE;
SELECT , , a FROM t;
SELECT a, , b FROM t;
INSERT INTO t VALUES (,);
INSERT INTO t VALUES (1, , 3);
DELETE FROM t WHERE = ;
DELETE FROM t WHERE AND;
DELETE FROM t WHERE OR;
UPDATE t SET a = AND;
UPDATE t SET a = OR;
UPDATE t SET a = FROM;
UPDATE t SET a = WHERE;
UPDATE t SET a = SET;
CREATE TABLE t (INTEGER);
SELECT a b FROM t;
SELECT a FROM t JOIN JOIN u;
SELECT a FROM t WHERE id = 1 AND AND b = 2;
SELECT a FROM t WHERE id = 1 OR OR b = 2;
INSERT INTO t VALUES (1, AND);
INSERT INTO t VALUES (1, OR);
INSERT INTO t VALUES (1, FROM);
INSERT INTO t VALUES (1, WHERE);
DELETE FROM t WHERE a = 1 AND = 2;
DELETE FROM t WHERE a = 1 OR = 2;
UPDATE t SET = = 1;
CREATE TABLE (id INTEGER);
SELECT a FROM t WHERE id = 1 2;
