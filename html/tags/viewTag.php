<?php
/**
 * @copyright Copyright (C) 2008 City of Bloomington, Indiana. All rights reserved.
 * @license http://www.gnu.org/copyleft/gpl.html GNU/GPL, see LICENSE.txt
 * @author Cliff Ingham <inghamn@bloomington.in.gov>
 * @param GET tag_id
 */
$template = new Template();

try
{
	$tag = new Tag($_GET['tag_id']);

	$template->blocks[] = new Block('tags/breadcrumbs.inc',array('tag'=>$tag));
	$template->blocks[] = new Block('tags/tagInfo.inc',array('tag'=>$tag));
	$template->blocks[] = new Block('topics/topicList.inc',array('topicList'=>$tag->getTopics()));
}
catch (Exception $e) { $_SESSION['errorMessages'][] = $e; }

echo $template->render();