<?php

namespace App\Entity;

use DateTime;
use Doctrine\ORM\Mapping as ORM;
use App\Repository\ActionRepository;
use App\Controller\ActionsUnresolved;
use App\Controller\ActionByLabelElement;
use App\Controller\ActionsByLabelElement;
use Doctrine\Common\Collections\Collection;
use ApiPlatform\Core\Annotation\ApiResource;
use Doctrine\Common\Collections\ArrayCollection;
use Symfony\Component\Serializer\Annotation\Groups;

/**
 * @ApiResource(
 *  normalizationContext={"groups" = {"action:read"}},
 *  denormalizationContext={"groups" = {"action:write"}},
 *  itemOperations={
 *     "get",
 *     "patch",
 *     "delete",
 *     "put",
 *     "get_unresolved_by_labelelement" = {
 *       "method" = "GET",
 *       "path" = "/actions/unresolved/{labelelement}",
 *       "controller" = ActionsByLabelElement::class,
 *       "read"=false,
 *       "openapi_context" = {
 *         "parameters" = {
 *           {
 *             "name" = "labelelement",
 *             "in" = "path",
 *             "description" = "The label of the element of the actions",
 *             "type" = "string",
 *             "required" = true,
 *             "example"= "label",
 *           },
 *         },
 *       },
 *     },
 *     "get_last_by_labelelement" = {
 *       "method" = "GET",
 *       "path" = "/action/last/{labelelement}",
 *       "controller" = ActionByLabelElement::class,
 *       "read"=false,
 *       "openapi_context" = {
 *         "parameters" = {
 *           {
 *             "name" = "labelelement",
 *             "in" = "path",
 *             "description" = "The label of the element of the action",
 *             "type" = "string",
 *             "required" = true,
 *             "example"= "label",
 *           },
 *         },
 *       },
 *     },
 *     "get_unresolved" = {
 *       "method" = "GET",
 *       "path" = "/actions/get/unresolved",
 *       "controller" = ActionsUnresolved::class,
 *       "read"=false,
 *     },
 *   },
 * )
 * @ORM\Entity(repositoryClass=ActionRepository::class)
 */
class Action
{
    /**
     * @ORM\Id
     * @ORM\GeneratedValue
     * @ORM\Column(type="integer")
     * @Groups({"action:read", "home"})
     */
    private $id;

    /**
     * @ORM\Column(type="string", length=191)
     * @Groups({"action:read", "action:write", "home:read"})
     */
    private $value;

    /**
     * @ORM\Column(type="datetime")
     * @Groups({"action:read", "home:read"})
     */
    private $datetime;

    /**
     * @ORM\Column(type="boolean")
     * @Groups({"action:read", "action:write", "home:read"})
     */
    private $state = false; // Not done by default

    /**
     * @ORM\ManyToOne(targetEntity=Element::class, inversedBy="actions")
     * @ORM\JoinColumn(nullable=false)
     * @Groups({"action:read", "action:write"})
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

        $this->element->setAction($this);

        return $this;
    }
}
