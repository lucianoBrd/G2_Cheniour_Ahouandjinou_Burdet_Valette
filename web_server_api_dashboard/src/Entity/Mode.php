<?php

namespace App\Entity;

use App\Controller\ModeActive;
use App\Controller\ModeByLabel;
use Doctrine\ORM\Mapping as ORM;
use App\Repository\ModeRepository;
use ApiPlatform\Core\Annotation\ApiResource;
use Symfony\Component\Serializer\Annotation\Groups;

/**
 * @ApiResource(
 *  normalizationContext={"groups" = {"mode"}},
 *  denormalizationContext={"groups" = {"mode"}},
 *  itemOperations={
 *     "get",
 *     "patch",
 *     "delete",
 *     "put",
 *     "get_by_label" = {
 *       "method" = "GET",
 *       "path" = "/mode/{label}",
 *       "controller" = ModeByLabel::class,
 *       "read"=false,
 *       "openapi_context" = {
 *         "parameters" = {
 *           {
 *             "name" = "label",
 *             "in" = "path",
 *             "description" = "The label of your mode",
 *             "type" = "string",
 *             "required" = true,
 *             "example"= "label",
 *           },
 *         },
 *       },
 *     },
 *     "get_mode_active" = {
 *       "method" = "GET",
 *       "path" = "/get/mode/active",
 *       "controller" = ModeActive::class,
 *       "read"=false,
 *       "openapi_context" = {
 *         "parameters" = {},
 *       },
 *     },
 *   },
 * )
 * @ORM\Entity(repositoryClass=ModeRepository::class)
 */
class Mode
{
    /**
     * @ORM\Id
     * @ORM\GeneratedValue
     * @ORM\Column(type="integer")
     * @Groups({"mode"})
     */
    private $id;

    /**
     * @ORM\Column(type="string", length=255)
     * @Groups({"mode"})
     */
    private $label;

    /**
     * @ORM\Column(type="boolean")
     * @Groups({"mode"})
     */
    private $state;

    public function __construct()
    {
        $this->state = false;
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

    public function getState(): ?bool
    {
        return $this->state;
    }

    public function setState(bool $state): self
    {
        $this->state = $state;

        return $this;
    }
}
