<?php
/**
 * @copyright 2009-2014 City of Bloomington, Indiana
 * @license http://www.gnu.org/licenses/agpl.txt GNU/AGPL, see LICENSE.txt
 * @author Cliff Ingham <inghamn@bloomington.in.gov>
 * @param Zend\Db\ResultSet $this->committees
 */
namespace Application\Models;

use Blossom\Classes\View;
use Application\Templates\Helpers\ButtonLink;

$userCanEdit = Person::isAllowed('committees', 'edit');
?>
<div>
	<h2><?php
			echo sprintf('%s, %s, %s, &amp; %s',
				$this->_(['labels.council'   , 'labels.councils'   , 2]),
				$this->_(['labels.commission', 'labels.commissions', 2]),
				$this->_(['labels.committee' , 'labels.committees' , 2]),
				$this->_(['labels.board'     , 'labels.boards'     , 2])
			);
			if ($userCanEdit) {
				$helper = $this->template->getHelper('buttonLink');
				echo $helper->buttonLink(
					BASE_URI.'/committees/update',
					$this->_('labels.add_committee'),
					'add'
				);
			}
		?>
	</h2>
	<table>
		<thead>
			<tr><th></th>
				<th><?php echo $this->_('labels.name'); ?></th>
				<th><?php echo $this->_(['labels.seat','labels.seats',2]); ?></th>
				<th></th>
			</tr>
		</thead>
		<tbody>
		<?php
			foreach ($this->committees as $c) {
				$name = View::escape($c->getName());
				$seats = $c->getMaxCurrentTerms();
				if (!$seats) { $seats = ''; }

				$editButton = '';
				if ($userCanEdit) {
					$editButton = $helper->buttonLink(
						BASE_URI.'/committees/update?committee_id='.$c->getId(),
						$this->_('labels.edit'),
						'edit',
						ButtonLink::SIZE_ICON
					);
				}
				$url = BASE_URI.'/committees/view?committee_id='.$c->getId();
				$vacancy = $c->hasVacancy() ? $this->_('labels.vacancy') : '';
				echo "
				<tr><td>$editButton</td>
					<td><a href=\"$url\">$name</a></td>
					<td>$seats</td>
					<td>$vacancy</td>
				</tr>
				";
			}
		?>
		</tbody>
	</table>
</div>
