<?php

namespace App\Controller;

use App\Entity\Type;
use Symfony\Component\HttpKernel\Attribute\AsController;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

#[AsController]
class TypeByLabel extends AbstractController
{
    public function __invoke(string $label)
    {
        $type = $this->getDoctrine()
            ->getRepository(Type::class)
            ->findOneBy(
                ['label' => $label],
            );
 
        if (!$type) {
            throw $this->createNotFoundException(
                'No type found for this label'
            );
        }
 
        return $type;
    }
}
