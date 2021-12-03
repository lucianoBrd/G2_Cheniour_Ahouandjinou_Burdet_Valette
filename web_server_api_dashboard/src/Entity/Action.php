<?php

namespace App\Entity;

use DateTime;
use Doctrine\ORM\Mapping as ORM;
use App\Repository\ActionRepository;
use Doctrine\Common\Collections\Collection;
use ApiPlatform\Core\Annotation\ApiResource;
use Doctrine\Common\Collections\ArrayCollection;
use Symfony\Component\Serializer\Annotation\Groups;

/**
 * @ApiResource(
 *  normalizationContext={"groups" = {"read"}},
 *  denormalizationContext={"groups" = {"write"}}
 * )
 * @ORM\Entity(repositoryClass=ActionRepository::class)
 */
class Action
{
    /**
     * @ORM\Id
     * @ORM\GeneratedValue
     * @ORM\Column(type="integer")
     * @Groups({"read"})
     */
    private $id;

    /**
     * @ORM\Column(type="string", length=191)
     * @Groups({"read", "write"})
     */
    private $value;

    /**
     * @ORM\Column(type="datetime")
     * @Groups({"read"})
     */
    private $datetime;

    /**
     * @ORM\Column(type="boolean")
     * @Groups({"read", "write"})
     */
    private $state = false; // Not done by default

    /**
     * @ORM\ManyToOne(targetEntity=Element::class, inversedBy="actions")
     * @ORM\JoinColumn(nullable=false)
     * @Groups({"read", "write"})
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

    public function getState(): ?bool
    {
        return $this->state;
    }

    public function setState(bool $state): self
    {
        $this->state = $state;

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
