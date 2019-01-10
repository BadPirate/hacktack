<?hh namespace Badpirate\HackTack;
class Model {
  static $db;

  static public function init(string $user, string $password, string $db) {
    $mysqli = new mysqli("localhost", $user, $password, $db);
    if ($mysqli->connect_errno) {
      throw new Exception("Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error);
    }
    Model::$db = $mysqli;
  }

  static public function dateTime(string $dateTimeString) : DateTime {
    return date('Y-m-d H:i:s',$dateTimeString);
  }

  static public function prepare(string $prep_string) : mysqli_stmt {
    if (!($stmt = Model::$db->prepare($prep_string)))
    {
      throw new Exception("Prepare failed: (" . Model::$db->errno . ")" . Model::$db->error);
    }
    return $stmt;
  }

  static public function affectedRows() : int {
    return Model::$db->affected_rows;
  }

  static public function execute(mysqli_stmt $stmt) {
    if (!$stmt->execute()) {
      throw new Exception("Execute failed: (" . $stmt->errno . ") " . $stmt->error);
    }
  }

  static public function ec(mysqli_stmt $stmt, bool $expect_changes = false) {
    Model::execute($stmt);
    if ($expect_changes && $stmt->affected_rows == 0) {
      throw new Exception("Expected change, 0 affected rows");
    }
    $stmt->close();
  }

  static public function generateRandomString($length = 10) {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
  }
}
