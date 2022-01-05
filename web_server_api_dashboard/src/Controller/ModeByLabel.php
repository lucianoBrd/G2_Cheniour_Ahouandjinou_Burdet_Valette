<?php

namespace App\Controller;

use App\Entity\Home;
use App\Entity\Mode;
use Symfony\Component\HttpKernel\Attribute\AsController;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

#[AsController]
class ModeByLabel extends AbstractController
{
    public function __invoke(string $labelhome, string $label)
    {
        $home = $this->getDoctrine()
            ->getRepository(Home::class)
            ->findOneBy(
                ['label' => $labelhome],
            );
 
        if (!$home) {
            throw $this->createNotFoundException(
                'No home found for this label'
            );
        }

        $mode = $this->getDoctrine()
            ->getRepository(Mode::class)
            ->findOneBy(
                ['label' => $label, 'home' => $home],
            );
 
        if (!$mode) {
            throw $this->createNotFoundException(
                'No mode found for this label'
            );
        }
 
        return $mode;
    }
}
