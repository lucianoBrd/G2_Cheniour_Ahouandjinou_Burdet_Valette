<?php

namespace App\Entity;

use App\Controller\TypeByLabel;
use Doctrine\ORM\Mapping as ORM;
use App\Repository\TypeRepository;
use Doctrine\Common\Collections\Collection;
use ApiPlatform\Core\Annotation\ApiResource;
use Doctrine\Common\Collections\ArrayCollection;

/**
 * @ApiResource(
 *  itemOperations={
 *     "get",
 *     "patch",
 *     "delete",
 *     "put",
 *     "get_by_label" = {
 *       "method" = "GET",
 *       "path" = "/type/{label}",
 *       "controller" = TypeByLabel::class,
 *       "read"=false,
 *       "openapi_context" = {
 *         "parameters" = {
 *           {
 *             "name" = "label",
 *             "in" = "path",
 *             "description" = "The label of your type",
 *             "type" = "string",
 *             "required" = true,
 *             "example"= "label",
 *           },
 *         },
 *       },
 *     },
 *   },
 * )
 * @ORM\Entity(repositoryClass=TypeRepository::class)
 */
class Type
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
     * @ORM\OneToMany(targetEntity=Element::class, mappedBy="type")
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
            $element->setType($this);
        }

        return $this;
    }

    public function removeElement(Element $element): self
    {
        if ($this->elements->removeElement($element)) {
            // set the owning side to null (unless already changed)
            if ($element->getType() === $this) {
                $element->setType(null);
            }
        }

        return $this;
    }
}
