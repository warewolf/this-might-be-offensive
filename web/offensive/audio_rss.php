<?
set_include_path("..");
require_once("offensive/assets/header.inc");

// this file has been deprecated as part of the audio tab removal process.  sorry :(
header('HTTP/1.0 410 Gone');
exit;

header( "Content-type: text/xml" ); 

// Include, and check we've got a connection to the database.
require_once( 'admin/mysqlConnectionInfo.inc' );
$link = openDbConnection();

mustLogIn("http");

?>
<rss version="2.0"
	xmlns:itunes="http://www.itunes.com/DTDs/Podcast-1.0.dtd" 	
>
	<channel>
		<title>tmbo.org : [ this might be offensive ]</title>
		<link>http://tmbo.org/offensive/</link>
		<description>[ this might be offensive ]</description>
		<lastBuildDate><? echo date("r"); ?></lastBuildDate>

<?	
	$sql = "select offensive_uploads.*, users.username
			FROM offensive_uploads
				LEFT JOIN users ON offensive_uploads.userid = users.userid
			WHERE type='audio'
			ORDER BY timestamp DESC
			LIMIT 200";

	$result = tmbo_query( $sql );

	while( $row = mysql_fetch_assoc( $result ) ) {
		if( file_exists( "images/audio/" . $row['filename'] ) ) {
?>
			<item>
				<title><![CDATA[<?= $row['filename']?> (uploaded by <? echo $row['username']?>)]]></title>
				<link>http://tmbo.org/offensive/images/audio/<? echo rawurlencode( $row['filename'] ) ?></link>
				<enclosure url="http://tmbo.org/offensive/images/audio/<?= rawurlencode( $row['filename'] ) ?>" length="<?= filesize( 'images/audio/' . $row['filename'] ) ?>" type="audio/mpeg"/>
				<description><![CDATA[ <?= $row['filename'] ?>]]></description>
				<pubDate><? echo date( "r", strtotime( $row['timestamp'] ) ) ?></pubDate>			
				<comments><![CDATA[http://tmbo.org/offensive/?c=comments&fileid=<? echo $row['id'] ?>]]></comments>
			</item>
<?
		}
	}
?>

	</channel>
</rss>