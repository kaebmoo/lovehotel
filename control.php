// kaebmoo 2017

<?php

use Mosquitto\Client;
use Mosquitto\Message;

class MqttToDb {

    /* Our Mosquitto\Client instance */
    /**
     * @var Client The MQTT client
     */
    private $mqtt;

    /**
     * @var PDO A PDO database connection
     */
    private $db;

    /**
     * @var array The list of topics to subscribe to
     */
    private $topics = [];

    /**
     * @var PDOStatement A prepared statement used when we record a message
     */
    private $insertMessage;

    /**
     * @param Client $mqtt The Mosquitto\Client instance
     * @param PDO $db a PDO database connection
     */
    public function __construct(Client $mqtt, PDO $db)
    {
        $this->mqtt = $mqtt;
        $this->db = $db;

        $this->insertMessage = $this->db->prepare(
            'INSERT INTO mqtt_logs (id, topic, payload, received) VALUES (?, ?, ?, NOW());'
        );

        /* Subscribe the Client to the topics when we connect */
        $this->mqtt->onConnect([$this, 'subscribeToTopics']);
        $this->mqtt->onMessage([$this, 'handleMessage']);
    }

    /**
     * @param array $topics
     *
     * An associative array of topics and their QoS values
     */
    public function setTopics(array $topics)
    {
        $this->topics = $topics;
    }

    /**
     * The internal callback used when the Client instance connects
     */
    public function subscribeToTopics() {
        foreach ($this->topics as $topic => $qos) {
            $this->mqtt->subscribe($topic, $qos);
        }
    }

    /**
     * @param Message $message
     * The internal callback used when a new message is received
     */
    public function handleMessage($message)
    {
        $this->insertMessage->execute([$message->mid, $message->topic, $message->payload]);
    }

    /**
     * Start recording messages
     */
    public function start()
    {
        $this->mqtt->loopForever();
    }
}

/* Create a new DB connection */
$db = new PDO('mysql:host=localhost;dbname=mqtt_logs;charset=utf8', 'mqtt_logs', 'mqtt_logs');

/* Configure our Client */
$mqtt = new Client();
$mqtt->connect('localhost');

$logger = new MqttToDb($mqtt, $db);
$logger->setTopics([
    '/room1/status' => 0,
    '/room2/status' => 0,
]);

$logger->start();

?>
