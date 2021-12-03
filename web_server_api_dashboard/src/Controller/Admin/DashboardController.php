<?php

namespace App\Controller\Admin;

use App\Entity\Home;
use App\Entity\Room;
use App\Entity\Type;
use App\Entity\Value;
use App\Entity\Action;
use App\Entity\Element;
use App\Controller\Admin\RoomCrudController;
use Symfony\Component\HttpFoundation\Response;
use Symfony\Component\Routing\Annotation\Route;
use EasyCorp\Bundle\EasyAdminBundle\Config\MenuItem;
use EasyCorp\Bundle\EasyAdminBundle\Config\Dashboard;
use EasyCorp\Bundle\EasyAdminBundle\Controller\AbstractDashboardController;

class DashboardController extends AbstractDashboardController
{
    /**
     * @Route("/", name="dashboard")
     */
    public function index(): Response
    {
        return parent::index();
    }

    public function configureDashboard(): Dashboard
    {
        return Dashboard::new()
            ->setTitle('Domotique')
            ->setFaviconPath('image/favicon.svg');
    }

    public function configureMenuItems(): iterable
    {
        yield MenuItem::linktoDashboard('Dashboard', 'fa fa-home');
        yield MenuItem::linkToCrud('Action', 'fa fa-play', Action::class);
        yield MenuItem::linkToCrud('Element', 'fa fa-robot', Element::class);
        yield MenuItem::linkToCrud('Home', 'fa fa-house-user', Home::class);
        yield MenuItem::linkToCrud('Room', 'fa fa-person-booth', Room::class)->setController(RoomCrudController::class);
        yield MenuItem::linkToCrud('Type', 'fa fa-spell-check', Type::class);
        yield MenuItem::linkToCrud('Value', 'fa fa-sort-numeric-up-alt', Value::class);
        yield MenuItem::linkToUrl('API', 'fa fa-road', '/api');
    }
}
