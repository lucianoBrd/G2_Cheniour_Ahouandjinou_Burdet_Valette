<?php

namespace App\Entity;

use ApiPlatform\Core\Annotation\ApiResource;
use App\Repository\ElementRepository;
use Doctrine\Common\Collections\ArrayCollection;
use Doctrine\Common\Collections\Collection;
use Doctrine\ORM\Mapping as ORM;

/**
 * @ApiResource(
 *  itemOperations={
 *     "get",
 *     "patch",
 *     "delete",
 *     "put",
 *     "get_by_label" = {
 *       "method" = "GET",
 *       "path" = "/element/{label}",
 *       "controller" = ElementByLabel::class,
 *       "read"=false,
 *       "openapi_context" = {
 *         "parameters" = {
 *           {
 *             "name" = "label",
 *             "in" = "path",
 *             "description" = "The label of your element",
 *             "type" = "string",
 *             "required" = true,
 *             "example"= "label",
 *           },
 *         },
 *       },
 *     },
 *   },
 * )
 * @ORM\Entity(repositoryClass=ElementRepository::class)
 */
class Element
{
    /**
     * @ORM\Id
     * @ORM\GeneratedValue
     * @ORM\Column(type="integer")
     */
    private $id;

    /**
     * @ORM\Column(type="string", length=255, unique=true)
     */
    private $label;

    /**
     * @ORM\Column(type="integer", nullable=true)
     */
    private $battery;

    /**
     * @ORM\ManyToOne(targetEntity=Room::class, inversedBy="elements")
     * @ORM\JoinColumn(nullable=false)
     */
    private $room;

    /**
     * @ORM\ManyToMany(targetEntity=Action::class, inversedBy="elements")
     */
    private $actions;

    /**
     * @ORM\OneToMany(targetEntity=Value::class, mappedBy="element", orphanRemoval=true)
     */
    private $elementValues;

    /**
     * @ORM\ManyToOne(targetEntity=Type::class, inversedBy="elements")
     * @ORM\JoinColumn(nullable=false)
     */
    private $type;

    public function __construct()
    {
        $this->actions = new ArrayCollection();
        $this->elementValues = new ArrayCollection();
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

    public function getBattery(): ?int
    {
        return $this->battery;
    }

    public function setBattery(?int $battery): self
    {
        $this->battery = $battery;

        return $this;
    }

    public function getRoom(): ?Room
    {
        return $this->room;
    }

    public function setRoom(?Room $room): self
    {
        $this->room = $room;

        return $this;
    }

    /**
     * @return Collection|Action[]
     */
    public function getActions(): Collection
    {
        return $this->actions;
    }

    public function addAction(Action $action): self
    {
        if (!$this->actions->contains($action)) {
            $this->actions[] = $action;
        }

        return $this;
    }

    public function removeAction(Action $action): self
    {
        $this->actions->removeElement($action);

        return $this;
    }

    /**
     * @return Collection|Value[]
     */
    public function getElementValues(): Collection
    {
        return $this->elementValues;
    }

    public function addElementValue(Value $elementValue): self
    {
        if (!$this->elementValues->contains($elementValue)) {
            $this->elementValues[] = $elementValue;
            $elementValue->setElement($this);
        }

        return $this;
    }

    public function removeElementValue(Value $elementValue): self
    {
        if ($this->elementValues->removeElement($elementValue)) {
            // set the owning side to null (unless already changed)
            if ($elementValue->getElement() === $this) {
                $elementValue->setElement(null);
            }
        }

        return $this;
    }

    public function getType(): ?Type
    {
        return $this->type;
    }

    public function setType(?Type $type): self
    {
        $this->type = $type;

        return $this;
    }
}
