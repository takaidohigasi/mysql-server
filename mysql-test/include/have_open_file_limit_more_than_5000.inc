--disable_query_log
--exec ulimit -n > $MYSQL_TMP_DIR/osfilelimit.txt
CREATE TABLE o_f_l (c1 varchar(20));
eval LOAD DATA INFILE '$MYSQL_TMP_DIR/osfilelimit.txt' INTO TABLE o_f_l;
let $f_l=`SELECT c1 FROM o_f_l`;
--remove_file $MYSQL_TMP_DIR/osfilelimit.txt
DROP TABLE o_f_l;
if ($f_l < 5000)
{
  skip Need open_files_limit >= 5000 (see ulimit -n);
}
--enable_query_log