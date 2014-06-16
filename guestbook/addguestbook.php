<?php
$host="localhost";// Host name 
$username="root";// Mysql username 
$password="";// Mysql password 
$db_name="guestbook";// Database name 
$tbl_name="guestbook";// Table name 

$name = $_POST['name'];
$email = $_POST['email'];
$comment = $_POST['comment'];
// Connect to server and select database.
mysql_connect("$host", "$username", "$password")or die("cannot connect server "); 
mysql_select_db("$db_name")or die("cannot select DB");

$datetime=date("y-m-d h:i:s"); //date time
$sql="INSERT INTO $tbl_name(name, email, comment, datetime)VALUES('$name', '$email', '$comment', '$datetime')";
$result=mysql_query($sql);

//check if query successful 
if($result){
echo "Successful";
echo "<BR>";
// link to view guestbook page
echo "<a href='viewguestbook.php'>View guestbook</a>";
}
else {
echo "ERROR";
}

mysql_close();
?>