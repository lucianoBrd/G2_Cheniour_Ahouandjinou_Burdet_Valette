<?php

namespace App\Controller;

use App\Entity\Action;
use App\Entity\Element;
use Symfony\Component\HttpKernel\Attribute\AsController;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

#[AsController]
class ActionsByLabelElement extends AbstractController
{
    public function __invoke(string $labelelement)
    {
        $element = $this->getDoctrine()
            ->getRepository(Element::class)
            ->findOneBy(
                ['label' => $labelelement],
            );
 
        if (!$element) {
            throw $this->createNotFoundException(
                'No element found for this label'
            );
        }

        $action = $this->getDoctrine()
            ->getRepository(Action::class)
            ->findBy(
                ['state' => false],
                ['element' => $element],
                ['datetime' => 'ASC']
            );
 
        if (!$action) {
            throw $this->createNotFoundException(
                'No action found for this element'
            );
        }
 
        return $action;
    }
}
