<?php
	class Seat
	{
		private $id;
		private $commission_id;
		private $category_id;
		private $title;
		private $vacancy;
		
		private $term_start;
		private $term_end;
		
		private $commission;
		private $category;
		private $restrictions;
		private $user;

		public function __construct($id=null)
		{
			global $PDO;

			if ($id)
			{
				if (is_numeric($id)) { $sql = "select * from seats where id=$id"; }
				else { $sql = "select * from seats where title='$id'"; }
				
				$result = $PDO->query($sql);
				if ($result)
				{
					if ($row = $result->fetch())
					{
						# This will load all fields in the table as properties of this class.
						# You may want to replace this with, or add your own extra, custom loading
						foreach($row as $field=>$value) { if ($value) $this->$field = $value; }


						$result->closeCursor();
					}
					else { throw new Exception($sql); }
				}
				else { $e = $PDO->errorInfo(); throw new Exception($sql.$e[2]); }
			}
			else
			{
				# This is where the code goes to generate a new, empty instance.
				# Set any default values for properties that need it here
			}
		}
		

		public function save()
		{
			# Check for required fields here.  Throw an exception if anything is missing.
	

			# This generates generic SQL that should work right away.
			# You can (and maybe should) replace this $fields code with your own custom SQL
			# for each property of this class,
			$fields = array();
			$fields[] = $this->commission_id ? "commission_id='{$this->commission_id}'" : "commission_id=null";
			$fields[] = $this->category_id ? "category_id='{$this->category_id}'" : "category_id=null";
			$fields[] = $this->title ? "title='{$this->title}'" : "title=null";
			$fields[] = $this->vacancy ? "vacancy='{$this->vacancy}'" : "vacancy=null";

			$fields = implode(",",$fields);


			if ($this->id) { $this->update($fields); }
			else { $this->insert($fields); }
			
			$this->updateRestrictions();
			$this->updateUser();
		}

		private function update($fields)
		{
			global $PDO;

			$sql = "update seats set $fields where id={$this->id}";
			if (false === $PDO->exec($sql)) { $e = $PDO->errorInfo(); throw new Exception($sql.$e[2]); }
		}

		private function insert($fields)
		{
			global $PDO;

			$sql = "insert seats set $fields";
			if (false === $PDO->exec($sql)) { $e = $PDO->errorInfo(); throw new Exception($sql.$e[2]); }
			$this->id = $PDO->lastInsertID();
		}
		
		private function updateRestrictions()
		{
			global $PDO;

			$restrictions = $this->getRestrictions();
		
			$sql = "delete from seat_restrictions where seat_id={$this->id}";
			if (false === $PDO->exec($sql)) { $e = $PDO->errorInfo(); throw new Exception($sql.$e[2]); }

			$statement = $PDO->prepare("insert seat_restrictions set seat_id={$this->id},restriction_id=?");
			foreach($restrictions as $restriction_id=>$restriction) 
			{ 
				if (false === $statement->execute(array("$restriction_id"))) { $e = $PDO->errorInfo(); throw new Exception($sql.$e[2]); }
			}
		}
		
		private function updateUser()
		{
			global $PDO;

			if($user = $this->user) 
			{
				$user_id = $user->getId();
			
				$sql = "delete from seat_users where seat_id={$this->id}";
				if (false === $PDO->exec($sql)) { $e = $PDO->errorInfo(); throw new Exception($sql.$e[2]); }

				$sql  = "insert seat_users set seat_id={$this->id},user_id=$user_id,term_start='{$this->term_start}',term_end='{$this->term_end}'";
				if (false === $PDO->exec($sql)) { $e = $PDO->errorInfo(); throw new Exception($sql.$e[2]); }
			}
			else 
			{
				$sql = "delete from seat_users where seat_id={$this->id}";
				if (false === $PDO->exec($sql)) { $e = $PDO->errorInfo(); throw new Exception($sql.$e[2]); }
			}
		}

		public function getId() { return $this->id; }
		public function getCommission_id() { return $this->commission_id; }
		public function getCategory_id() { return $this->category_id; }
		public function getTitle() { return $this->title; }
		public function getVacancy() { return $this->vacancy; }
		public function getCommission() 
		{
			if (!$this->commission) { $this->commission = new Commission($this->commission_id); }
			return $this->commission;
		}
			
		public function getCategory() 
		{
			if (!$this->category) { $this->category = new SeatCategory($this->category_id); }
			return $this->category;
		}
		
		public function getRestrictions() 
		{
			global $PDO;

			if (!$this->restrictions)
			{
				$this->restrictions = array();
				$sql = "select restriction_id,restriction from seat_restrictions left join restrictions on restriction_id=id where seat_id={$this->id}";
				if ($result = $PDO->query($sql))
				{
					foreach($result as $row) { $this->restrictions[$row['restriction_id']] = $row['restriction']; }
					$result->closeCursor();
				}
			}
			return $this->restrictions;
		}
		
		public function getTermStart() 
		{
			global $PDO;
			if (!$this->term_start)
			{
				$sql = "select term_start from seat_users left join users on user_id=id where seat_id={$this->id}"; 
				if ($result = $PDO->query($sql))
				{
					foreach($result as $row) { $this->term_start = $row['term_start']; }
					$result->closeCursor();
				}
			}
			return $this->term_start;
		}
		
		public function getTermEnd() 
		{
			global $PDO;
			if (!$this->term_end)
			{
				$sql = "select term_end from seat_users left join users on user_id=id where seat_id={$this->id}"; 
				if ($result = $PDO->query($sql))
				{
					foreach($result as $row) { $this->term_end = $row['term_end']; }
					$result->closeCursor();
				}
			}
			return $this->term_end;
		}
		
		public function getUser() 
		{
			global $PDO;

			if (!$this->user && $this->vacancy != 1)
			{
				$sql = "select user_id from seat_users left join users on user_id=id where seat_id={$this->id}";
				if ($result = $PDO->query($sql))
				{
					foreach($result as $row) { $id =  $row['user_id']; }
					$result->closeCursor();
				}
				$this->user = new User($id);
			}
			return $this->user;
		}

		public function setId($int) { $this->id = $int; }
		public function setCommission_id($int) { $this->commission_id = $int; }
		public function setCategory_id($int) { $this->category_id = $int; }
		public function setTitle($string) { $this->title = $string; }
		public function setVacancy($int) { $this->vacancy = $int; }
		public function addVacancy($int) { if (!isset($this->vacancy)) $this->vacancy = $int; }
		public function removeVacancy($int) { if (isset($this->vacancy)) unset($this->vacancy); }
		public function setTermStart($string) { $this->term_start = sanitizeString($string); }
		public function setTermEnd($string) { $this->term_end = sanitizeString($string); }
		public function setUser($user) 
		{ 
			$this->user = $user; 
		}
		public function unsetUser() { if (isset($this->user)) unset($this->user); }
		public function setCommission($commission) 
		{ 
			$this->commission = $commission; 
			$this->commission_id = $commission->getId(); 
		}
		
		public function setCategory($category) 
		{ 
			$this->category = $category; 
			$this->category_id = $category->getId(); 
		}
		
		public function setRestrictions($array) 
		{ 
			$this->restrictions = array();
			foreach($array as $id)
			{
				$restrictions = new Restriction($id);
				$this->restrictions[$restrictions->getId()] = $restrictions->getRestriction();
			} 
		}
		
		public function deleteSeat() 
		{
			global $PDO;

			$sql = "delete from seat_users where seat_id ={$this->id}";
			if (false === $PDO->exec($sql)) { $e = $PDO->errorInfo(); throw new Exception($sql.$e[2]); }
			
			$sql = "delete from seat_restrictions where seat_id ={$this->id}";
			if (false === $PDO->exec($sql)) { $e = $PDO->errorInfo(); throw new Exception($sql.$e[2]); }

			$sql = "delete from seats where id ={$this->id}";
			if (false === $PDO->exec($sql)) { $e = $PDO->errorInfo(); throw new Exception($sql.$e[2]); }
		}

	}
?>