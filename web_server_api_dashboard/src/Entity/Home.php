<?php

namespace App\Entity;

use App\Controller\HomeByLabel;
use Doctrine\ORM\Mapping as ORM;
use App\Repository\HomeRepository;
use Doctrine\Common\Collections\Collection;
use ApiPlatform\Core\Annotation\ApiResource;
use Doctrine\Common\Collections\ArrayCollection;
use Symfony\Component\Serializer\Annotation\Groups;

/**
 * @ApiResource(
 *  normalizationContext={"groups" = {"home"}},
 *  denormalizationContext={"groups" = {"home"}},
 *  itemOperations={
 *     "get",
 *     "patch",
 *     "delete",
 *     "put",
 *     "get_by_label" = {
 *       "method" = "GET",
 *       "path" = "/home/{label}",
 *       "controller" = HomeByLabel::class,
 *       "read"=false,
 *       "openapi_context" = {
 *         "parameters" = {
 *           {
 *             "name" = "label",
 *             "in" = "path",
 *             "description" = "The label of your home",
 *             "type" = "string",
 *             "required" = true,
 *             "example"= "label",
 *           },
 *         },
 *       },
 *     },
 *   },
 * )
 * @ORM\Entity(repositoryClass=HomeRepository::class)
 */
class Home
{
    /**
     * @ORM\Id
     * @ORM\GeneratedValue
     * @ORM\Column(type="integer")
     * @Groups({"home"})
     */
    private $id;

    /**
     * @ORM\Column(type="string", length=191, unique=true)
     * @Groups({"home"})
     */
    private $label;

    /**
     * @ORM\OneToMany(targetEntity=Room::class, mappedBy="home", orphanRemoval=true)
     * @Groups({"home"})
     */
    private $rooms;

    public function __construct()
    {
        $this->rooms = new ArrayCollection();
    }

    public function __toString()
    {
        return $this->label;
    }

    public function getId(): ?int
    {
        return $this->id;
    }

    public function getLabel(): ?string
    {
        return $this->label;
    }

    public function setLabel(string $label): self
    {
        $this->label = $label;

        return $this;
    }

    /**
     * @return Collection|Room[]
     */
    public function getRooms(): Collection
    {
        return $this->rooms;
    }

    public function addRoom(Room $room): self
    {
        if (!$this->rooms->contains($room)) {
            $this->rooms[] = $room;
            $room->setHome($this);
        }

        return $this;
    }

    public function removeRoom(Room $room): self
    {
        if ($this->rooms->removeElement($room)) {
            // set the owning side to null (unless already changed)
            if ($room->getHome() === $this) {
                $room->setHome(null);
            }
        }

        return $this;
    }
}
