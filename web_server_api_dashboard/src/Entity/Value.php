<?php

namespace App\Entity;

use DateTime;
use Doctrine\ORM\Mapping as ORM;
use App\Repository\ValueRepository;
use App\Controller\ValueByLabelElement;
use ApiPlatform\Core\Annotation\ApiResource;
use Symfony\Component\Serializer\Annotation\Groups;

/**
 * @ApiResource(
 *  normalizationContext={"groups" = {"value:read"}},
 *  denormalizationContext={"groups" = {"value:write"}},
 *  itemOperations={
 *     "get",
 *     "patch",
 *     "delete",
 *     "put",
 *     "get_last_by_labelelement" = {
 *       "method" = "GET",
 *       "path" = "/value/last/{labelelement}",
 *       "controller" = ValueByLabelElement::class,
 *       "read"=false,
 *       "openapi_context" = {
 *         "parameters" = {
 *           {
 *             "name" = "labelelement",
 *             "in" = "path",
 *             "description" = "The label of the element of the value",
 *             "type" = "string",
 *             "required" = true,
 *             "example"= "label",
 *           },
 *         },
 *       },
 *     },
 *   },
 * )
 * @ORM\Entity(repositoryClass=ValueRepository::class)
 */
class Value
{
    /**
     * @ORM\Id
     * @ORM\GeneratedValue
     * @ORM\Column(type="integer")
     * @Groups({"value:read", "home:read"})
     */
    private $id;

    /**
     * @ORM\Column(type="string", length=191)
     * @Groups({"value:read", "value:write", "home:read"})
     */
    private $value;

    /**
     * @ORM\Column(type="datetime")
     * @Groups({"value:read", "home:read"})
     */
    private $datetime;

    /**
     * @ORM\ManyToOne(targetEntity=Element::class, inversedBy="elementValues")
     * @ORM\JoinColumn(nullable=true, onDelete="SET NULL")
     * @Groups({"value:read", "value:write"})
     */
    private $element;

    public function __construct() 
    {
        $this->datetime = new \DateTime('now');
    }

    public function __toString()
    {
        return $this->value;
    }

    public function getId(): ?int
    {
        return $this->id;
    }

    public function getValue(): ?string
    {
        return $this->value;
    }

    public function setValue(string $value): self
    {
        $this->value = $value;

        return $this;
    }

    public function getDatetime(): ?\DateTimeInterface
    {
        return $this->datetime;
    }

    public function setDatetime(\DateTimeInterface $datetime): self
    {
        $this->datetime = $datetime;

        return $this;
    }

    public function getElement(): ?Element
    {
        return $this->element;
    }

    public function setElement(?Element $element): self
    {
        $this->element = $element;
        
        $this->element->setElementValue($this);

        return $this;
    }
}
