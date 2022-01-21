<?php

namespace App\Controller;

use App\Entity\Home;
use App\Entity\Mode;
use Symfony\Component\HttpKernel\Attribute\AsController;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

#[AsController]
class ModeActive extends AbstractController
{
    public function __invoke(string $labelhome)
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
        
        $modes = $this->getDoctrine()
            ->getRepository(Mode::class)
            ->findBy(
                ['state' => true, 'home' => $home],
                [],
                1
            );
 
        /*if (!$values) {
            throw $this->createNotFoundException(
                'No value found for this element'
            );
        }*/

        foreach ($modes as $mode) {
            return $mode;
        }
 
        return null;
    }
}
