<?
	// Include, and check we've got a connection to the database.
	require_once( 'admin/mysqlConnectionInfo.inc' );
	if(!isset($link) || !$link) $link = openDbConnection();
	require_once( 'offensive/assets/functions.inc' );

	function start() {
		mustLogIn();

		if( array_key_exists("submit", $_REQUEST) && array_key_exists("topic", $_REQUEST) ) {
			$topic = trim( $_REQUEST['topic'] );
			if( strlen( $topic ) > 0 ) {
				$tid = createTopic($topic);
				
				id(new Upload($tid))->subscribe();
				
				if(array_key_exists("comment", $_REQUEST)) {
					core_postcomment(array("fileid" => $tid, "comment" => $_REQUEST['comment']));
				}
				
				header("Location: ".Link::thread($tid));
				exit;
			}
		}
	}
	
	function createTopic($topic) {
		$uid = me()->id();
		$sql = "INSERT INTO offensive_uploads ( userid, filename, ip, type )
					VALUES ( $uid, '".sqlEscape($topic)."', '" . $_SERVER['REMOTE_ADDR'] . "', 'topic' )";
		tmbo_query($sql);

	 	$threadid = mysql_insert_id();
	
		$sql = "INSERT INTO offensive_count_cache(threadid, good, bad, tmbo, comments, repost) VALUES($threadid, 0, 0, 0, 0, 0)";
		tmbo_query($sql);
		
		insertChange("upload", $threadid, "topic" . ":" . me()->username() . ":" . $topic);

		return $threadid;

	}
	
	require_once( 'offensive/assets/tabs.inc' );

	function body() {
?>
<div class="contentbox">
	<div class="blackbar"></div>
		<div class="heading">we need to talk.</div>

<?
		global $activeTab;
		
		$activeTab = "discussions";

		tabs();
?>
		<div class="bluebox">
		
			<form method="post" action="<?php echo $_SERVER['PHP_SELF'] ?>">

				<div style="text-align:center">
					<div class="label">about:
					<input type="text" name="topic" size="50" />
					<input type="hidden" name="c" value="newtopic"/>					
					</div>
					<textarea name="comment" style="width:80%;height:150px;"></textarea>
					<br/>
					<input type="submit" name="submit" value="go"/>
				</div>
				
			</form>
			
		</div>
		<div class="blackbar"></div>
	</div>
<?
	}
?>