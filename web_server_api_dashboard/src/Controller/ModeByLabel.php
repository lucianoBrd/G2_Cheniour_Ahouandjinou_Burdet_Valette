<?php

namespace App\Controller;

use App\Entity\Mode;
use Symfony\Component\HttpKernel\Attribute\AsController;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

#[AsController]
class ModeByLabel extends AbstractController
{
    public function __invoke(string $label)
    {
        $mode = $this->getDoctrine()
            ->getRepository(Mode::class)
            ->findOneBy(
                ['label' => $label],
            );
 
        if (!$mode) {
            throw $this->createNotFoundException(
                'No mode found for this label'
            );
        }
 
        return $mode;
    }
}
