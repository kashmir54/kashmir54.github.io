 <?php require 'config.php';

class db extends Connection {
    public function query($sql) {
        $args = func_get_args();
        unset($args[0]);
        return parent::query(vsprintf($sql, $args));
    }
}

function sanitize($d, $s) {
    if (preg_match_all('/'. implode('|', array(
        'in', 'or', 'and', 'set', 'file',
    )). '/i', $s, $matches)) die(var_dump($matches[0]));
    return mysqli_real_escape_string($d->conn(), $s);
}

$db = new db();

if (isset($_POST['pass'])) {
    $pass = sanitize($db, $_POST['pass']);
    $db->query("SELECT * FROM users WHERE password=('$pass') AND username=('%s')", 'admin');
} else {
    die(highlight_file(__FILE__,1));
} 