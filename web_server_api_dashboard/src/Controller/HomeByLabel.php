<?php

namespace App\Controller;

use App\Entity\Home;
use Symfony\Component\HttpKernel\Attribute\AsController;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

#[AsController]
class HomeByLabel extends AbstractController
{
    public function __invoke(string $label)
    {
        $home = $this->getDoctrine()
            ->getRepository(Home::class)
            ->findOneBy(
                ['label' => $label],
            );
 
        if (!$home) {
            throw $this->createNotFoundException(
                'No home found for this label'
            );
        }
 
        return $home;
    }
}
