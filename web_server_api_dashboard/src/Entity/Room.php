<?php

namespace App\Entity;

use App\Controller\RoomByLabel;
use Doctrine\ORM\Mapping as ORM;
use App\Repository\RoomRepository;
use Doctrine\Common\Collections\Collection;
use ApiPlatform\Core\Annotation\ApiResource;
use Doctrine\Common\Collections\ArrayCollection;
use Symfony\Component\Serializer\Annotation\Groups;

/**
 * @ApiResource(
 *  normalizationContext={"groups" = {"room"}},
 *  denormalizationContext={"groups" = {"room"}}
 *  itemOperations={
 *     "get",
 *     "patch",
 *     "delete",
 *     "put",
 *     "get_by_label" = {
 *       "method" = "GET",
 *       "path" = "/room/{label}",
 *       "controller" = RoomByLabel::class,
 *       "read"=false,
 *       "openapi_context" = {
 *         "parameters" = {
 *           {
 *             "name" = "label",
 *             "in" = "path",
 *             "description" = "The label of your room",
 *             "type" = "string",
 *             "required" = true,
 *             "example"= "label",
 *           },
 *         },
 *       },
 *     },
 *   },
 * )
 * @ORM\Entity(repositoryClass=RoomRepository::class)
 */
class Room
{
    /**
     * @ORM\Id
     * @ORM\GeneratedValue
     * @ORM\Column(type="integer")
     * @Groups({"room"})
     */
    private $id;

    /**
     * @ORM\Column(type="string", length=191, unique=true)
     * @Groups({"room"})
     */
    private $label;

    /**
     * @ORM\ManyToOne(targetEntity=Home::class, inversedBy="rooms")
     * @ORM\JoinColumn(nullable=false)
     * @Groups({"room"})
     */
    private $home;

    /**
     * @ORM\OneToMany(targetEntity=Element::class, mappedBy="room", orphanRemoval=true)
     * @Groups({"room"})
     */
    private $elements;

    public function __construct()
    {
        $this->elements = new ArrayCollection();
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

    public function getHome(): ?Home
    {
        return $this->home;
    }

    public function setHome(?Home $home): self
    {
        $this->home = $home;

        return $this;
    }

    /**
     * @return Collection|Element[]
     */
    public function getElements(): Collection
    {
        return $this->elements;
    }

    public function addElement(Element $element): self
    {
        if (!$this->elements->contains($element)) {
            $this->elements[] = $element;
            $element->setRoom($this);
        }

        return $this;
    }

    public function removeElement(Element $element): self
    {
        if ($this->elements->removeElement($element)) {
            // set the owning side to null (unless already changed)
            if ($element->getRoom() === $this) {
                $element->setRoom(null);
            }
        }

        return $this;
    }
}
