<?hh
require_once('vendor/autoload.php');
require_once('vendor/hh_autoload.php');

// A bootstrap head
final class :head:jstrap extends :x:element {
  attribute :head;
  attribute string width = "device-width";

  use XHPHelpers;

  protected function render(): XHPRoot {
    $id = $this->getID();

    $head = 
      <head>
        <meta charset="utf-8" />
        <meta name="viewport" content={"width=".$this->:width.", initial-scale=1, shrink-to-fit=no"} />
        <script src="//code.jquery.com/jquery-1.11.0.min.js"></script>
        <!-- Latest compiled and minified CSS -->
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css"
         crossorigin="anonymous" />
        <link rel="stylesheet" href="//code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css"/>
        <script src="https://code.jquery.com/jquery-3.2.1.min.js" crossorigin="anonymous"></script>
        <script src="https://code.jquery.com/ui/1.12.1/jquery-ui.min.js" crossorigin="anonymous"></script>
        <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js" crossorigin="anonymous"></script>
        <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.5.0/css/all.css" crossorigin="anonymous"/>
        <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.5.0/css/brands.css" crossorigin="anonymous"/>
        {$this->getChildren()}
      </head>;

    $this->transferAllAttributes($head);

    return $head;
  }
}