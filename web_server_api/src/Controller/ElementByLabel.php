<?php

namespace App\Controller;

use App\Entity\Element;
use Symfony\Component\HttpKernel\Attribute\AsController;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

#[AsController]
class ElementByLabel extends AbstractController
{
    public function __invoke(string $label)
    {
        $element = $this->getDoctrine()
            ->getRepository(Element::class)
            ->findOneBy(
                ['label' => $label],
            );
 
        if (!$element) {
            throw $this->createNotFoundException(
                'No element found for this label'
            );
        }
 
        return $element;
    }
}
