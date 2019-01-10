<?hh

class Badpirate\HackTack\HT {
  static public function redirect(string $url) {
    header("Location: $url");
    die();
  }
}