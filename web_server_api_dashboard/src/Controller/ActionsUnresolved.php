<?php

namespace App\Controller;

use App\Entity\Action;
use Symfony\Component\HttpKernel\Attribute\AsController;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

#[AsController]
class ActionsUnresolved extends AbstractController
{
    public function __invoke()
    {
        $actions = $this->getDoctrine()
            ->getRepository(Action::class)
            ->findBy(
                ['state' => false],
                ['datetime' => 'ASC']
            );
 
        if (!$actions) {
            throw $this->createNotFoundException(
                'No actions found for this element'
            );
        }
 
        return $actions;
    }
}
