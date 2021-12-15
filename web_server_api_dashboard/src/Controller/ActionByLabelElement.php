<?php

namespace App\Controller;

use App\Entity\Action;
use App\Entity\Element;
use Symfony\Component\HttpKernel\Attribute\AsController;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

#[AsController]
class ActionByLabelElement extends AbstractController
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

        $actions = $this->getDoctrine()
            ->getRepository(Action::class)
            ->findBy(
                ['element' => $element],
                ['datetime' => 'DESC'],
                1
            );
 
        /*if (!$values) {
            throw $this->createNotFoundException(
                'No value found for this element'
            );
        }*/

        foreach ($actions as $action) {
            return $action;
        }
 
        return null;
    }
}
