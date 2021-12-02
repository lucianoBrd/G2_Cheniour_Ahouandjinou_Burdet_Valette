<?php

namespace App\Controller;

use App\Entity\Room;
use Symfony\Component\HttpKernel\Attribute\AsController;
use Symfony\Bundle\FrameworkBundle\Controller\AbstractController;

#[AsController]
class RoomByLabel extends AbstractController
{
    public function __invoke(string $label)
    {
        $room = $this->getDoctrine()
            ->getRepository(Room::class)
            ->findOneBy(
                ['label' => $label],
            );
 
        if (!$room) {
            throw $this->createNotFoundException(
                'No room found for this label'
            );
        }
 
        return $room;
    }
}
