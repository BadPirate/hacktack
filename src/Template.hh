<?hh namespace Badpirate\HackTack;

final class FormField {
  public function __construct(
    public string $name,
    public string $id,
    public string $type,
    public string $placeholder,
    public ?string $value = null
  ) {}
}

Template::$cli = (php_sapi_name() === "cli");

class Template {
  static public bool $cli = false;
  public ?:xhp $jumbotron;
  public ?:xhp $body;
  public :xhp $head;

  public ?:xhp $headers;
  public bool $target_errored = false;

  public function __construct(
    public string $title
  ) 
  {
    $this->head =
      <head>
        <meta charset="utf-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
        <script src="//code.jquery.com/jquery-1.11.0.min.js"></script>
        <!-- Latest compiled and minified CSS -->
        <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css"
          crossorigin="anonymous" />
        <link rel="stylesheet" href="//code.jquery.com/ui/1.12.1/themes/base/jquery-ui.css"/>
        <script src="https://code.jquery.com/jquery-3.2.1.min.js" crossorigin="anonymous"></script>
        <script src="https://code.jquery.com/ui/1.12.1/jquery-ui.min.js" crossorigin="anonymous"></script>
        <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js" crossorigin="anonymous"></script>
        <script src="https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js" crossorigin="anonymous"></script>
        <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.5.0/css/all.css" crossorigin="anonymous"/>
        <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.5.0/css/brands.css" crossorigin="anonymous"/>
        <link rel="stylesheet" href="template.min.css"/>
        <script src="template.min.js"></script>
        <title>{$this->title}</title>
      </head>;
  }

  static public function timePeriodString(int $time) {
    $amount = 0;
    $unit = "";
    if($time > 86400) {
      $unit = "day";
      $amount = floor($time/86400);
    } else if ($time > 3600) {
      $unit = "hour";
      $amount = floor($time/3600);
    } else {
      $unit = "minute";
      $amount = floor($time/60);
    }
    return "$amount $unit".($amount > 1 ? 's' : '');
  }

  static public function redirect(string $url) {
    header("Location: $url");
    die();
  }

  static public string $app_name = "HackTack";

  static public function fatalError(string $error, ?Map<string,string> $extra = null) {
    if (Template::$cli) {
      d($error);
      var_dump($extra);
      die(0);
    }
    $t = new Template("Error");
    $extra = $extra ?? Map {};
    $extra['error'] = $error;
    ob_start();
    $error = error_get_last() ?? $error;
    d($error);
    $debug = ob_get_contents();
    ob_end_clean();
    $path = rtrim(sys_get_temp_dir(),'/')."/".Template::$app_name."_error_".basename($_SERVER['SCRIPT_NAME'])."_".(date(DateTime::ATOM)).".log";
    file_put_contents($path,$debug);
    $extra['file'] = $path;
    $t->body = 
      <div class="alert alert-danger">
        <h5>Error :(</h5>
        <pre>{$error}</pre>
        {$debug}
      </div>;
    $t->output(true);
    die(0);
  }

  public function targetError(string $error) {
    print
    <div class={"alert alert-warning".($this->target_errored ? "" : " mt-3")}>
      {$error}
    </div>;
    $this->target_errored = true;
  }

  public function targetRedirect(string $url) {
    print "REDIRECT: $url";
    die();
  }

  public function output(bool $skipTarget = false) : void {
    if (Template::$cli) { throw new Exception("Template output from CLI"); }

    print "<!doctype html />";
    print
    <html lang="en">
     {$this->head}
     <body>
       {($this->jumbotron !== null) ?
         <bootstrap:jumbotron>
         {$this->jumbotron}
         </bootstrap:jumbotron> : null}
       {($this->body !== null) ?
         <div class="px-3 py-3">{$this->body}</div> : null}
     </body>
    </html>;
    die();
  }

  public function form(string $header,
                       Vector <FormField> $fields,
                       ?Map<string,string> $hidden_values,
                       string $submit_title,
                       string $target,
                       ?string $page = null,
                       ?:xhp $error_message = null) : :xhp
  {
    $card_body = <div class="card-body"/>;
    foreach($fields as $field) {
      $card_body->appendChild(
        <div class="form-group">
          <label for={$field->id}>
            {$field->name}
          </label>
          <input type={$field->type}
                 class="form-control"
                 name={$field->id}
                 placeholder={$field->placeholder}
                 value={$field->value ?? ""}/>
        </div>
      );
    }
    if($hidden_values) {
      foreach($hidden_values as $key=>$value) {
        $card_body->appendChild(
          <input type="hidden" name={$key} value={$value}/>
        );
      }
    }
    return
    <form method="post" action={($page ? "$page" : "")."?target=$target" }>
      <div class="card">
        <div class="card-header h5">
          {$header}
        </div>
        {$card_body}
        {
          $error_message ?
          <div class="alert alert-warning">
            {$error_message}
          </div> : null
        }
        <input type="submit"
               class="btn btn-primary"
               value={$submit_title}/>
      </div>
    </form>;
  }

  public function target(array $target) {
    throw new Exception("No target handler on class");
  }

  public function formAction(string $target,
                             ?Map<string,string> $values = null,
                             ?Map<string,string> $formValues = null,
                             ?string $alt_page = null) : string
  {
    $form_action = "javascript:formaction(this, '$target', '".($alt_page ?? "")."?target=$target";
    if ($values) {
      foreach($values as $name=>$value) {
        $value = urlencode($value);
        $form_action .= "&$name=$value";
      }
    }
    if ($formValues)
    {
      foreach($formValues as $name=>$value) {
        $form_action .= "&$name=' + encodeURIComponent(\$(\"#input-$value\").val()) + '";
      }
    }
    $form_action .= "')";
    return $form_action;
  }
}

class :x:raw extends :x:primitive {
	category %flow, %metadata, %phrase;
	protected function stringify() {
		$buf = '';
		foreach ($this->getChildren() as $child) {
			if (!is_string($child)) {
				throw new XHPClassException($this, 'Child must be a string');
			}
			$buf .= (string) $child;
		}
		return $buf;
	}
}
