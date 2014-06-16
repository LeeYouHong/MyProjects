<table width="400" border="0" align="center" cellpadding="3" cellspacing="0">
<tr>
<td><strong>View Guestbook | <a href="index.php">Sign Guestbook</a> </strong></td>
</tr>
</table>
<br>
<?php
$host="localhost";// Host name
$username="root";// Mysql username
$password="";// Mysql password
$db_name="guestbook";// Database name
$tbl_name="guestbook";// Table name 
$comment_count = 0;
$page_size = 3;
$page_now = empty($_GET["page"]) ? 1 : $_GET["page"];
$page_count = 0;

// Connect to server and select database.
mysql_connect("$host", "$username", "$password")or die("cannot connect server "); 
mysql_select_db("$db_name")or die("cannot select DB");

// Get count of comment
$sql = "SELECT COUNT(*) FROM $tbl_name";
$result = mysql_query($sql);
if($rows = mysql_fetch_array($result)) {
    $comment_count = $rows[0];
}
// Get count of page
$page_count = ceil($comment_count/$page_size);

// Get content of current page
$comment_start = $page_size * ($page_now-1);
$sql="SELECT * FROM $tbl_name limit $comment_start,$page_size";
$result=mysql_query($sql);

while($rows=mysql_fetch_array($result)){
?>
<table width="400" border="0" align="center" cellpadding="0" cellspacing="1" bgcolor="#CCCCCC">
<tr>
<td><table width="400" border="0" cellpadding="3" cellspacing="1" bgcolor="#FFFFFF">
<tr>
<td>ID</td>
<td>:</td>
<td><?php echo $rows['id']; ?></td>
</tr>
<tr>
<td width="117">Name</td>
<td width="14">:</td>
<td width="357"><?php echo $rows['name']; ?></td>
</tr>
<tr>
<td>Email</td>
<td>:</td>
<td><?php echo $rows['email']; ?></td>
</tr>
<tr>
<td valign="top">Comment</td>
<td valign="top">:</td>
<td><?php echo $rows['comment']; ?></td>
</tr>
<tr>
<td valign="top">Date/Time </td>
<td valign="top">:</td>
<td><?php echo $rows['datetime']; ?></td>
</tr>
</table></td>
</tr>
</table>
<?php
}
?>
<table width="400" border="0" align="center" cellpadding="3" cellspacing="0">
<tr>
<td align="center">
<?php
for ($i=1; $i <= $page_count; $i++)
{
	if ($i == $page_now)
	{
	    echo "<a>$i</a>&nbsp";
	} else {
	    echo "<a href='viewguestbook.php?page=$i'>$i</a>&nbsp";
	}
}
?>
</td>
</tr>
</table>
<?php
mysql_close(); //close database
?>
