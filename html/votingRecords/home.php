<?php
/**
 * @copyright Copyright (C) 2006-2008 City of Bloomington, Indiana. All rights reserved.
 * @license http://www.gnu.org/copyleft/gpl.html GNU/GPL, see LICENSE.txt
 */
$votingRecordList = new VotingRecordList();
$votingRecordList->find();

$template = new Template();
$template->blocks[] = new Block('votingRecords/votingRecordList.inc',array('votingRecordList'=>$votingRecordList));
echo $template->render();