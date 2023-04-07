$connection = new PDO('mysql:host=localhost;dbname=database', 'username', 'password');
$connection->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

// Path: test/Model.php
class Model
{
    protected $connection;

    public function __construct(PDO $connection)
    {
        $this->connection = $connection;
    }
}
