<?php

namespace App\Entity;

use DateTime;
use Doctrine\ORM\Mapping as ORM;
use App\Repository\ValueRepository;
use ApiPlatform\Core\Annotation\ApiResource;
use Symfony\Component\Serializer\Annotation\Groups;

/**
 * @ApiResource(
 *  normalizationContext={"groups" = {"read"}},
 *  denormalizationContext={"groups" = {"write"}}
 * )
 * @ORM\Entity(repositoryClass=ValueRepository::class)
 */
class Value
{
    /**
     * @ORM\Id
     * @ORM\GeneratedValue
     * @ORM\Column(type="integer")
     * @Groups({"read"})
     */
    private $id;

    /**
     * @ORM\Column(type="string", length=255)
     * @Groups({"read", "write"})
     */
    private $value;

    /**
     * @ORM\Column(type="datetime")
     * @Groups({"read"})
     */
    private $datetime;

    /**
     * @ORM\ManyToOne(targetEntity=Element::class, inversedBy="elementValues")
     * @ORM\JoinColumn(nullable=false)
     * @Groups({"read", "write"})
     */
    private $element;

    public function __construct() 
    {
        $this->datetime = new \DateTime('now');
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

        return $this;
    }
}
