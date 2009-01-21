<?php
/**
 * @copyright Copyright (C) 2006-2008 City of Bloomington, Indiana. All rights reserved.
 * @license http://www.gnu.org/copyleft/gpl.html GNU/GPL, see LICENSE.txt
 */
$voteTypeList = new VoteTypeList();
$voteTypeList->find();

$template = new Template();
$template->blocks[] = new Block('voteTypes/voteTypeList.inc',array('voteTypeList'=>$voteTypeList));
echo $template->render();