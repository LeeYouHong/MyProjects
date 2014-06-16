<html>
<head>
<title>Test Sign Guestbook</title>
<link rel="stylesheet" href="assets/css/table/index.css" type="text/css">
</head>
<body>
<div id="wrap">
        <h1 id="title">Test Sign Guestbook</h1>
	<form id="guestform" name="guestform" method="post" action="addguestbook.php">
            <div id="name">
		<span id="name_label">Name:</span>
		<div id="name_input">
			<input name="name" class="input_area" type="text"/>
		</div>
	    </div>
            <div id="email">
	        <span id="email_label">Email:</span>
		<div id="email_input">
			<input name="email"  class="input_area" type="text"/>
		</div>
            </div>
            <div id="comment">
	        <span id="comment_label">Comment:</span>
		<div id="comment_textarea">
			<textarea name="comment" class="input_area"></textarea>
		</div>
            </div>
            <div id="operate">
                    <input type="submit" name="Submit" value="Submit" onClick="return checkForm();"/> 
                    <input type="reset" name="Submit2" value="Reset" />
            </div>
        </form>
        <div id="footer"><a id="view_guestbook" href="viewguestbook.php">View Guestbook</a></div>
</div>
</body>
</html>
<script>
function checkForm()
{
    var gname, gemail, gcomment;
	with(window.document.guestform)
	{
	    gname = name;
		gemail = email;
		gcomment = comment;
	}
	if (trim(gname.value) == '')
	{
	    alert('Please enter your name');
		gname.focus();
		return false;
	} else if (trim(gemail.value) == '')
	{
	    alert('Please enter your email');
		gemail.focus();
		return false;
	} else if ( !isEmail(trim(gemail.value)) )
	{
	    alert('Please enter a valid email address');
		gemail.focus();
		return false;
	} else if (trim(gcomment.value) == '')
	{
	    alert('Please enter your comment');
		gcomment.focus();
		return false;
	} else 
	{
	    return true;
	}
}
function trim(str)
{
    return str.replace(/^\s+|\s+$/g, '');
}

function isEmail(str)
{
    var regex = /^[-_.a-z0-9]+@(([-a-z0-9]+\.)+(ad|ae|aero|af|ag|\ai|al|am|an|ao|aq|ar|arpa|as|at|au|aw|az|ba|bb|bd|be|bf|bg|bh|bi|biz|bj|bm|bn|bo|br|bs|bt|bv|bw|by|bz|ca|cc|cd|cf|cg|ch|ci|ck|cl|cm|cn|co|com|coop|cr|cs|cu|cv|cx|cy|cz|de|dj|dk|dm|do|dz|ec|edu|ee|eg|eh|er|es|et|eu|fi|fj|fk|fm|fo|fr|ga|gb|gd|ge|gf|gh|gi|gl|gm|gn|gov|gp|gq|gr|gs|gt|gu|gw|gy|hk|hm|hn|hr|ht|hu|id|ie|il|in|info|int|io|iq|ir|is|it|jm|jo|jp|ke|kg|kh|ki|km|kn|kp|kr|kw|ky|kz|la|lb|lc|li|lk|lr|ls|lt|lu|lv|ly|ma|mc|md|mg|mh|mil|mk|ml|mm|mn|mo|mp|mq|mr|ms|mt|mu|museum|mv|mw|mx|my|mz|na|name|nc|ne|net|nf|ng|ni|nl|no|np|nr|nt|nu|nz|om|org|pa|pe|pf|pg|ph|pk|pl|pm|pn|pr|pro|ps|pt|pw|py|qa|re|ro|ru|rw|sa|sb|sc|sd|se|sg|sh|si|sj|sk|sl|sm|sn|so|sr|st|su|sv|sy|sz|tc|div|tf|tg|th|tj|tk|tm|tn|to|tp|tr|tt|tv|tw|tz|ua|ug|uk|um|us|uy|uz|va|vc|ve|vg|vi|vn|vu|wf|ws|ye|yt|yu|za|zm|zw)|(([0-9][0-9]?|[0-1][0-9][0-9]|[2][0-4][0-9]|[2][5][0-5])\.){3}([0-9][0-9]?|[0-1][0-9][0-9]|[2][0-4][0-9]|[2][5][0-5]))$/i;
	return regex.test(str);
}
</script>
