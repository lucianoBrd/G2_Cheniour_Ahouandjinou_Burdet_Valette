<?php

namespace App\Entity;

use Doctrine\ORM\Mapping as ORM;
use App\Controller\ElementByLabel;
use App\Repository\ElementRepository;
use Doctrine\Common\Collections\Collection;
use ApiPlatform\Core\Annotation\ApiResource;
use Doctrine\Common\Collections\ArrayCollection;
use Symfony\Component\Serializer\Annotation\Groups;

/**
 * @ApiResource(
 *  normalizationContext={"groups" = {"element"}},
 *  denormalizationContext={"groups" = {"element"}},
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
     * @Groups({"element", "home:read"})
     */
    private $id;

    /**
     * @ORM\Column(type="string", length=191, unique=true)
     * @Groups({"element", "home:read"})
     */
    private $label;

    /**
     * @ORM\Column(type="integer", nullable=true)
     * @Groups({"element", "home:read"})
     */
    private $battery;

    /**
     * @ORM\ManyToOne(targetEntity=Room::class, inversedBy="elements")
     * @ORM\JoinColumn(nullable=true)
     * @Groups({"element"})
     */
    private $room;

    /**
     * @ORM\OneToMany(targetEntity=Value::class, mappedBy="element", orphanRemoval=true, cascade={"remove"})
     * @Groups({"element"})
     */
    private $elementValues;

    /**
     * @ORM\ManyToOne(targetEntity=Type::class, inversedBy="elements")
     * @ORM\JoinColumn(nullable=true)
     * @Groups({"element", "home:read"})
     */
    private $type;

    /**
     * @ORM\OneToMany(targetEntity=Action::class, mappedBy="element", orphanRemoval=true, cascade={"remove"})
     * @Groups({"element"})
     */
    private $actions;

    /**
     * @ORM\Column(type="boolean", nullable=true)
     * @Groups({"element", "home:read"})
     */
    private $state;

    /**
     * @ORM\OneToOne(targetEntity=Action::class, cascade={"persist", "remove"})
     * @Groups({"element", "home:read"})
     */
    private $action = null;

    /**
     * @ORM\OneToOne(targetEntity=Value::class, cascade={"persist", "remove"})
     * @Groups({"element", "home:read"})
     */
    private $elementValue = null;

    public function __construct()
    {
        $this->state = false;
        $this->elementValues = new ArrayCollection();
        $this->actions = new ArrayCollection();
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
            $this->elementValue = $elementValue;
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
            $this->action = $action;
            $action->setElement($this);
        }

        return $this;
    }

    public function removeAction(Action $action): self
    {
        if ($this->actions->removeElement($action)) {
            // set the owning side to null (unless already changed)
            if ($action->getElement() === $this) {
                $action->setElement(null);
            }
        }

        return $this;
    }

    public function getState(): ?bool
    {
        return $this->state;
    }

    public function setState(?bool $state): self
    {
        $this->state = $state;

        return $this;
    }

    public function getAction(): ?Action
    {
        return $this->action;
    }

    public function setAction(?Action $action): self
    {
        $this->action = $action;

        return $this;
    }

    public function getElementValue(): ?Value
    {
        return $this->elementValue;
    }

    public function setElementValue(?Value $elementValue): self
    {
        $this->elementValue = $elementValue;

        return $this;
    }
}
