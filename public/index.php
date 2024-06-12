<?php
declare(strict_types=1);

use DI\Container;
use \Psr\Http\Message\ServerRequestInterface as Request;
use \Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Server\RequestHandlerInterface;
use Slim\Factory\AppFactory;
use Slim\Routing\RouteContext;

require '../vendor/autoload.php';

$container = new Container();
AppFactory::setContainer($container);
$container->set('logger', function (){
    return new \Slim\Logger();
});

$app = AppFactory::create();

$app->addBodyParsingMiddleware();
// This middleware will append the response header Access-Control-Allow-Methods with all allowed methods
$app->add(function (Request $request, RequestHandlerInterface $handler): Response {
    $routeContext = RouteContext::fromRequest($request);
    $routingResults = $routeContext->getRoutingResults();
    $methods = $routingResults->getAllowedMethods();

    $response = $handler->handle($request);

    $response = $response->withHeader('Access-Control-Allow-Origin', '*');
    $response = $response->withHeader('Access-Control-Allow-Methods', implode(',', $methods));
    $response = $response->withHeader('Access-Control-Allow-Headers', '*');
    $response = $response->withHeader('Access-Control-Allow-Credentials', 'true');

    return $response;
});


$app->addRoutingMiddleware();
$app->addErrorMiddleware(true, true, true);

$app->options('/format', function (Request $request, Response $response) {
    return $response;
});

$app->post('/format', function (Request $request, Response $response, $args) {
    $body = $request->getParsedBody();
    $format_str = $body['format'];
    $format_str_args = $body['args'];
    $cmd = "/var/www/formatter/formatter '$format_str' '" . implode("' '", $format_str_args) . "'";
    $this->get('logger')->info("cmd: $cmd");
    exec($cmd, $output, $status);
    $this->get('logger')->info("status: $status");
    $this->get('logger')->info("output: " . implode(" ", $output));
    $response->getBody()->write(json_encode(['command' => $cmd, 'status' => $status, 'output' => implode("\n", $output)]));
    $response = $response->withHeader('Content-Type', 'application/json');
    return $response;
});

$app->run();
